import csv
import sys


def check_match(strs, matches):
    """ Return true if the dna matches """

    for i in range(len(strs)):
        if int(strs[i]) != matches[i]:
            return False

    return True


def main():

    # TODO: Check for command-line usage
    if len(sys.argv) != 3:
        sys.exit("Usage: python dna.py data.csv sequence.txt")

    # TODO: Read database file into a variable
    csv_file = open(sys.argv[1], 'r')
    csv_reader = csv.reader(csv_file)

    # TODO: Read DNA sequence file into a variable
    seq_file = open(sys.argv[2], 'r')
    dna = seq_file.readline()
    seq_file.close()

    # TODO: Find longest match of each STR in DNA sequence
    longest_matches = next(csv_reader)[1:]
    for i in range(len(longest_matches)):
        longest_matches[i] = longest_match(dna, longest_matches[i])

    # TODO: Check database for matching profiles
    for line in csv_reader:
        if check_match(line[1:], longest_matches) is True:
            print(line[0])
            return

    print("No Match")


def longest_match(sequence, subsequence):
    """Returns length of longest run of subsequence in sequence."""

    # Initialize variables
    longest_run = 0
    subsequence_length = len(subsequence)
    sequence_length = len(sequence)

    # Check each character in sequence for most consecutive runs of subsequence
    for i in range(sequence_length):

        # Initialize count of consecutive runs
        count = 0

        # Check for a subsequence match in a "substring" (a subset of characters) within sequence
        # If a match, move substring to next potential match in sequence
        # Continue moving substring and checking for matches until out of consecutive matches
        while True:

            # Adjust substring start and end
            start = i + count * subsequence_length
            end = start + subsequence_length

            # If there is a match in the substring
            if sequence[start:end] == subsequence:
                count += 1

            # If there is no match in the substring
            else:
                break

        # Update most consecutive matches found
        longest_run = max(longest_run, count)

    # After checking for runs at each character in seqeuence, return longest run found
    return longest_run


main()
