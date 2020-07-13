# TODO
import csv
import sys
import re
import cs50


def main():

    # request command-line argument and validate for 3 arguments.
    if (len(sys.argv) != 2):

        print("Usage: python import.py filename.csv")
        sys.exit()

    # open db file for sqlite.
    db = cs50.SQL("sqlite:///students.db")

    # open csv file and read into a list in memory, then close it.
    with open(sys.argv[1], "r") as students_csv:

        csv_list = list(csv.reader(students_csv))
        csv_header = csv_list.pop(0)[1:]

    # iterate over all rows in csv file.
    for row in range(len(csv_list)):

        # splits name into a list (first, middle, last).
        name = csv_list[row][0].split()

        # determines if student has middle name.
        if (len(name) == 3):
            first = name[0]
            middle = name[1]
            last = name[2]
        else:
            first = name[0]
            middle = None
            last = name[1]

        house = csv_list[row][1]
        birth = csv_list[row][2]

        # inserts each row into db.
        db.execute("INSERT INTO students (first, middle, last, house, birth) VALUES(?, ?, ?, ?, ?)",
                   first, middle, last, house, birth)


main()