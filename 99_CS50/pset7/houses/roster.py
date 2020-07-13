
import csv
import sys
import re
import cs50


def main():

    # request command-line argument and validate for 2 arguments.
    if (len(sys.argv) != 2):

        print("Usage: python roster.py house")
        sys.exit()

    # store command line argument under house variable.
    house = sys.argv[1]

    # open db file for sqlite.
    db = cs50.SQL("sqlite:///students.db")

    # puts db query into a list.
    house_list = db.execute("SELECT first, middle, last, birth FROM students WHERE house = (?) ORDER BY last, first", house)

    # prints names and birth year, considering presence of middle name.
    for row in house_list:
        if row['middle'] == None:
            print(f"{row['first']} {row['last']}, born {row['birth']}")
        else:
            print(f"{row['first']} {row['middle']} {row['last']}, born {row['birth']}")


main()