from cs50 import get_float

# Prompt user for change value
while True:
    change = get_float("Number: ")
    if change >= 0:
        break

# Round input value and convert to cents
change = round(change, 2) * 100
coins = [25, 10, 5, 1]
total_coins = 0

# Loop over each available coin
for coin in coins:
    coins_number, change = divmod(change, coin)
    total_coins += int(coins_number)

print(total_coins)