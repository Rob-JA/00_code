# checks if credit card number is a valid number using luhn's algorithm.
#
#   luhn's algorithm
#   ----------------
#   Multiply every other digit by 2, starting with the number’s second-to-last digit, and then add those products’ digits together.
#   Add the sum to the sum of the digits that weren’t multiplied by 2.
#   If the total’s last digit is 0 (or, put more formally, if the total modulo 10 is congruent to 0), the number is valid!

counter = 0      # counter will be used to count number of digits.
checksum1 = 0    # checksum1 will be the odd numbers starting with last digit. (1st, 3rd, 5th...)
checksum2 = 0    # checksum2 will be every other number starting with 2nd to last. (2nd, 4th, 6th...)

# gets cc number from user.
while True:
    try:
        ccnumber = int(input("Number: "))
    except ValueError:
        continue
    else:
        break

number = ccnumber

while ((int(number / 10) > 0) or (number % 10 > 0)):
    odd = number % 10
    number = int(number / 10)
    checksum1 = checksum1 + odd
    counter += 1

    if (int(number / 10) > 0) or (number % 10 > 0):
        even = number % 10 * 2
        integer = int(even / 10)
        remain = even % 10
        even = integer + remain

        checksum2 = checksum2 + even
        number = int(number / 10)
        counter += 1

# isolates last 2 digits.
for i in range(counter - 2):
    ccnumber = int(ccnumber / 10)
    i += 1

# tests checksum condition.
if ((checksum1 + checksum2) % 10) != 0:
    print("INVALID")
    exit()

# tests amex conditions.
if counter == 15 and (ccnumber == 34 or ccnumber == 37):
    print("AMEX")

# tests mastercard conditions.
elif counter == 16 and ccnumber >= 51 and ccnumber <= 55:
    print("MASTERCARD")

# tests visa conditions. further isolates last digit.
elif counter == 13 or counter == 16 and (int(ccnumber / 10) == 4):
    print("VISA")

else:
    print("INVALID")