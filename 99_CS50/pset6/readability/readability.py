# This algorithm takes users text and calculates the Coleman-Liau index.
# The Coleman-Liau index of a text is designed to output what (U.S.) grade level is needed to understand the text.
# The formula is: index = 0.0588 * L - 0.296 * S - 15.8

letters = 0
words = 1
sentences = 0

# gets text from user.
text = str(input("Text: "))

# iterates through each character, counting letters, words, and sentences.
for i in text:
    if (i.isalpha()):
        letters += 1
    if (i.isspace()):
        words += 1
    if i in [".", "!", "?"]:
        sentences += 1

# calculates variables l and s for Coleman-Liau index.   
l = letters / words * 100
s = sentences * 100 / words
grade = round(0.0588 * l - 0.296 * s - 15.8)

# prints grade value with conditions where: (1 > grade >= 16).
if (grade >= 16):
    print("Grade 16+")
elif (grade < 1):
    print("Before Grade 1")
else:
    print(f"Grade {grade}")