import cs50
import csv

from pathlib import Path

from flask import Flask, jsonify, redirect, render_template, request

# Configure application
app = Flask(__name__)

# Reload templates when they are changed
app.config["TEMPLATES_AUTO_RELOAD"] = True


@app.after_request
def after_request(response):
    """Disable caching"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/", methods=["GET"])
def get_index():
    return redirect("/form")


# Outputs form
@app.route("/form", methods=["GET"])
def get_form():
    return render_template("form.html")


# Handle sumbitted form data
@app.route("/form", methods=["POST"])
def post_form():
    # Validate data, output error message if it is invalid
    error = validate_form()
    if error:
        return render_template("error.html", message=error)
    write_csv()
    return redirect("/sheet")


# Validates submitted form data
def validate_form():
    error = ""
    if not request.form.get("name"):
        error = "Game name must be set."
    if not request.form.get("year"):
        error = "Year must be set."
    if not request.form.get("genre"):
        error = "Game genre must be set."
    if not request.form.get("rating"):
        error = "Game rating must be set."
    return error


def write_csv():
    with open('survey.csv', "a") as file:
        writer = csv.writer(file)
        writer.writerow((
            request.form.get("name"),
            request.form.get("year"),
            request.form.get("genre"),
            request.form.get("rating")
        ))


# Output saved data
@app.route("/sheet", methods=["GET"])
def get_sheet():
    return render_template("table.html", rows=read_csv())


def read_csv():
    with open('survey.csv', "r") as file:
        reader = csv.reader(file)
        return list(reader)