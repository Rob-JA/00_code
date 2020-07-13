# get pyramid height, ensure valid input.
height = 0
while height < 1 or height > 8:
    try:
        height = int(input('Height: '))
    except ValueError:
        continue

# print spaces and hashes
for i in range(1, height + 1):
    j = height - i
    print(" " * j, end='')
    print("#" * i)