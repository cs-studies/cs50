from nltk.tokenize import sent_tokenize


def lines(a, b):
    """Return lines in both a and b"""
    a_lines = a.split("\n")
    b_lines = b.split("\n")
    return list(set(a_lines) & set(b_lines))


def sentences(a, b):
    """Return sentences in both a and b"""
    a_sentences = sent_tokenize(a)
    b_sentences = sent_tokenize(b)
    return list(set(a_sentences) & set(b_sentences))


def substrings(a, b, n):
    """Return substrings of length n in both a and b"""
    a_substrings = find_substrings(a, n)
    b_substrings = find_substrings(b, n)
    return list(a_substrings & b_substrings)


def find_substrings(s, n):
    """"Return set of subtrings of a given length"""
    substrings = set()
    for i in range(0, len(s) - n + 1):
        substrings.add(s[i:(n + i)])
    return substrings
