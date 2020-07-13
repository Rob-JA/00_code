# compares a csv of dna str's against a long dna string.
# references the csv to find the correct number of repeating dna str's and return the owner of dna.
# import modules

import csv
import sys
import re


def main():

    # request command-line argument and validate for 3 arguments
    if (len(sys.argv) != 3):

        print("Usage: python dna.py data.csv sequence.txt")
        sys.exit(0)

    # open csv file and read into a list in memory, then close it.
    with open(sys.argv[1], "r") as str_csv:
        
        str_csv_list = list(csv.reader(str_csv))
        str_seq = str_csv_list.pop(0)[1:]

    # open dna sequence file and read.
    seq_text = open(sys.argv[2], "r")
    dna_seq = seq_text.read()

    temp = 0
    temp_max = 0
    max_str = []

    # iterate over each str
    for i in range(len(str_seq)):

        j = 0
        temp_max = 0

        # loop over the long string of dna.
        while j < len(dna_seq):

            # count if str repeats consecutively.
            if (str_seq[i] == dna_seq[j:j + len(str_seq[i])]):
                
                temp += 1

                if (j + len(str_seq[i]) < len(dna_seq)):
                    
                    j = j + len(str_seq[i])
                    continue

            # if the str stops repeating, compare the current run (temp) vs the longest run (temp_variable).
            else:
                
                if temp > temp_max:
                    temp_max = temp
                
                temp = 0

            # keep searching by 1 in dna string until str found again.
            j += 1

        max_str.append(temp_max)

    str_match_count = 0

    # searches csv file and matches the str counts with the temp_max values.
    for person in range(len(str_csv_list)):

        for n in range(1, len(str_csv_list[person])):

            # if temp_max does not match csv value, reset count.
            if (max_str[n - 1] != int(str_csv_list[person][n])):
                str_match_count = 0

            # if temp_max does match csv value, increment count.
            if (max_str[n - 1] == int(str_csv_list[person][n])):
                str_match_count += 1

            # if all temp_max values match the csv values, return name of person.
            if (str_match_count == len(str_seq)):
                print(str_csv_list[person][0])
                sys.exit(1)

    # if no match is found return no match.
    print("No Match")
    sys.exit(0)


main()
