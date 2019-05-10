import os

import sys

from cs50 import SQL
from flask import Flask, flash, jsonify, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions, HTTPException, InternalServerError
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd, format_date, validate_stock_shares, load_stocks

# Configure application
app = Flask(__name__)


# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True


# Ensure responses aren't cached
@app.after_request
def after_request(response):
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""
    stocks = load_stocks(db, session["user_id"])
    total = 0
    for item in stocks:
        item_data = lookup(item['symbol'])
        if item_data is None:
            return apology(f"Failed to load data for {item['symbol']}", 400)
        item['price'] = item_data['price']
        item['name'] = item_data['name']
        item['total'] = int(item['sum_shares']) * item['price']
        total += item['total']
        item['total'] = usd(item['total'])
        item['price'] = usd(item['price'])

    users = db.execute("SELECT cash FROM users WHERE id=:id", id=session["user_id"])
    total += users[0]['cash']
    return render_template("stocks.html", rows=stocks, total=usd(total), user_cash=usd(users[0]['cash']))


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""

    if request.method == "POST":

        # Validate sumbitted data
        symbol = request.form.get("symbol")
        shares = request.form.get("shares")
        validationError = validate_stock_shares(symbol, shares)
        if validationError is not None:
            return validationError

        symbol = symbol.upper()
        shares = int(shares)
        # Lookup a requested stock
        quote = lookup(symbol)
        if quote is None:
            return apology(f"Failed to load data for {symbol}", 400)

        rows = db.execute("SELECT cash FROM users WHERE id = :id", id=session["user_id"])

        if len(rows) != 1:
            return apology("Failed to load user cash value", 500)

        user_cash = float(rows[0]['cash'])
#        print(f"user_cash: {user_cash}", file=sys.stderr)
        stock_price = float(quote['price'])
        shares_price = stock_price * shares

        # Validate if a user may buy shares at the current price
        if shares_price > user_cash:
            return apology(f"You cannot afford buying {shares} shares of {symbol}")

        db.execute("""
            INSERT INTO 'transaction' (user_id, symbol, shares, price)
            VALUES(:id, :symbol, :shares, :price)""", id=session["user_id"], symbol=symbol, shares=shares, price=stock_price)

        cash = user_cash - shares_price
        db.execute("UPDATE users SET cash=:cash WHERE id=:id", cash=cash, id=session["user_id"])

        return redirect("/")
    else:
        max_stocks = ""
        symbol = request.args.get("symbol", default="")
        if symbol:
            stock_data = lookup(symbol)
            if stock_data is not None:
                user_data = db.execute("SELECT cash FROM users WHERE id=:id", id=session['user_id'])
                stock_price = float(stock_data['price'])
                user_cash = float(user_data[0]['cash'])
                if stock_price < user_cash:
                    max_stocks = int(user_cash // stock_price)
        return render_template("buy.html", symbol=symbol, max_stocks=max_stocks)


@app.route("/check", methods=["GET"])
def check():
    """Return true if username available, else false, in JSON format"""
    valid = False
    username = request.args.get("username")
    if username:
        valid = not is_duplicate(username)
    return jsonify(valid)

def is_duplicate(username):
    return bool(db.execute("SELECT id FROM users WHERE username=:name LIMIT 1", name=username))

@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    data = db.execute("SELECT * FROM 'transaction' WHERE user_id=:id ORDER BY created DESC", id=session["user_id"])
    for item in data:
        item['price'] = usd(item['price'])
        item['created'] = format_date(item['created'])
    # print(item, file=sys.stderr)
    return render_template("history.html", rows=data)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username=:username", username=request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    if request.method == "POST":
        # Validate symbol is passed
        if not request.form.get("symbol"):
            return apology("Symbol must be set", 400)
        symbol = request.form.get("symbol")
        quote = lookup(symbol)
        if quote is None:
            return apology(f"Failed to load data for {symbol}", 400)
        quote['priceFormatted'] = usd(quote['price'])
        return render_template("quoted.html", quote=quote)
    else:
        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        username = request.form.get("username")
        password = request.form.get("password")

        # Ensure username was submitted
        if not username:
            return apology("Username must be provided", 400)

        # Validate username
        elif is_duplicate(username):
            return apology("Username {username} is already taken", 400)

        # Ensure password was submitted
        elif not password:
            return apology("Password must be provided", 400)

        # Ensure password matches password confirmation
        elif password != request.form.get("confirmation"):
            return apology("Password and password confirmation must be the same", 400)

        # Require strong password
        elif len(password) < 8:
            return apology("Password must be at least 8 characters long", 400)
        elif password.isalpha():
            return apology("Password must include at least one number or special character", 400)
        elif password.islower():
            return apology("Password must include at least one upper case letter", 400)
        elif password.isupper():
            return apology("Password must include at least one lower case letter", 400)

        # Insert new user into a database
        password_hash = generate_password_hash(request.form.get("password"))
        user_id = db.execute("INSERT INTO users (username, hash) VALUES (:username, :hash)", username=username, hash=password_hash)

        if not user_id:
            return apology(f"Failed to create a new user account", 500)

        # Automatically log in a just registered user
        session["user_id"] = user_id

        # Redirect user to home page
        return redirect("/quote")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""

    if request.method == "POST":

        # Validate sumbitted data
        symbol = request.form.get("symbol")
        shares = request.form.get("shares")
        validationError = validate_stock_shares(symbol, shares)
        if validationError is not None:
            return validationError

        symbol = symbol.upper()
        shares = int(shares)

        # Load user shares number
        rows = db.execute("""
            SELECT sum(shares) shares
            FROM 'transaction'
            WHERE user_id=:id AND symbol=:symbol""", id=session["user_id"], symbol=symbol)
        if len(rows) != 1:
            return apology("Failed to load user shares value", 500)
        user_shares = int(rows[0]['shares'])

        # Validate a user has enough shares to sell
        if shares > user_shares:
            return apology(f"You cannot sell more than {user_shares} shares of {symbol}")

        # Lookup a requested stock
        quote = lookup(symbol)
        if quote is None:
            return apology(f"Failed to load data for {symbol}", 400)

        stock_price = float(quote['price'])
        shares_price = stock_price * shares

        db.execute("""
            INSERT INTO 'transaction' (user_id, symbol, shares, price)
            VALUES(:id, :symbol, :shares, :price)""", id=session["user_id"], symbol=symbol, shares=-shares, price=stock_price)

        db.execute("UPDATE users SET cash=:cash + cash WHERE id=:id", cash=shares_price, id=session["user_id"])

        return redirect("/")
    else:
        max_stocks = ""
        symbol = request.args.get("symbol", default="")
        if symbol:
            user_data = db.execute("""
                SELECT sum(shares) sum_shares
                FROM 'transaction'
                WHERE user_id=:id AND symbol=:symbol
                LIMIT 1""", id=session['user_id'], symbol=symbol)

            if user_data and user_data[0]['sum_shares'] is not None:
                max_stocks = int(user_data[0]['sum_shares'])

        return render_template("sell.html", symbol=symbol, rows=load_stocks(db, session["user_id"]), max_stocks=max_stocks)


def errorhandler(e):
    """Handle error"""
    if not isinstance(e, HTTPException):
        e = InternalServerError()
    return apology(e.name, e.code)


# Listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)
