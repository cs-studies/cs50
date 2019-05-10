from cs50 import get_int

# Prompt user for a half-pyramid's height
while True:
    height = get_int("Height: ")
    if height > 0 and height < 9:
        break

# Loop over rows
for row in range(height):
    spaces = height - (row + 1)
    print(" " * spaces, "#" * (height - spaces), sep="")