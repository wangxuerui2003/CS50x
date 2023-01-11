# TODO

from cs50 import *


# Get index of readability


def get_index(text):
    words = 1
    sentences = 0
    letters = 0

    for c in text:
        if c == ' ':
            words += 1
        elif c == '.' or c == '!' or c == '?':
            sentences += 1
        elif c.isalpha():
            letters += 1

    index = 0.0588 * (letters / words * 100) - 0.296 * (sentences / words * 100) - 15.8

    return index

# get the text from stdin


text = get_string("Text: ")
index = int(round(get_index(text)))


# Determine readability
if index < 1:
    print("Before Grade 1")
elif index > 16:
    print("Grade 16+")
else:
    print("Grade", index)

