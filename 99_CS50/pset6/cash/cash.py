# derives the minimum number of coins required to provide correct change.

dollars = -1
coins = 0

# gets required change from user.
while dollars < 0:
    try:
        dollars = float(input("Change owed: "))
    except ValueError:
        continue

# turns dollars in to cents and rounds.
cents = round(dollars * 100)

# calc 25c
while (cents - 25 >= 0):
    coins += 1
    cents -= 25

# calc 10c
while (cents - 10 >= 0):
    coins += 1
    cents -= 10

# calc 5c
while (cents - 5 >= 0):
    coins += 1
    cents -= 5

# calc 1c
while (cents - 1 >= 0):
    coins += 1
    cents -= 1

print(coins)
