# TODO

from cs50 import *

# function for check sum


def check_sum(card_num):
    sum = 0

    while card_num != 0:
        sum += card_num % 10
        card_num //= 10
        temp = card_num % 10 * 2
        sum += temp % 10
        if temp >= 10:
            sum += 1
        card_num //= 10

    return (sum % 10 == 0)

# check which bank is the card
# print invalid if the card number is invalid


def check_bank(card_num):
    clen = len(card_num)

    if clen == 15 and (card_num[0:2] == '34' or card_num[0:2] == '37'):
        print("AMEX")
    elif (clen == 16 or clen == 13) and card_num[0] == '4':
        print("VISA")
    elif clen == 16 and (int(card_num[0:2]) >= 51 and int(card_num[0:2]) <= 55):
        print("MASTERCARD")
    else:
        print("INVALID")


# main logic
card_num = get_int("Number: ")
if check_sum(card_num):
    check_bank(str(card_num))
else:
    print("INVALID")
