import random
import string


def random_string(symbols, size):
    return ''.join([random.choice(symbols) for _ in range(size)])

def random_eng_string(size = 10):
    return random_string(string.ascii_letters, size)


def random_ru_string(size = 10):
    letters = "йцукенгшщзхъёфывапролджэячсмитьбю"
    return random_string(letters, size)


def random_digit_string(size = 10):
    digits = "1234567890"
    return random_string(digits, size)
