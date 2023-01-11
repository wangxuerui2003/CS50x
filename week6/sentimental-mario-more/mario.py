# TODO

from cs50 import get_int

height = 0

# keep prompting if invalid height
while height < 1 or height > 8:
    height = get_int("Height: ")

# start output the pyramid
for i in range(1, height + 1):
    # no newline using end
    print(' ' * (height - i), end='')
    print('#' * i, end='')
    print('  ', end='')
    print('#' * i)
