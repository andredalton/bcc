# coding=utf-8
"""
Nome: André Meneghelli Vale
NUSP: 4898948
Problema P32 - B. Cows and Poker Game
"""


def main():
    n = int(raw_input())
    cows = list(raw_input())

    As = 0
    Is = 0
    for cow in cows:
        if cow == "A":
            As += 1
        if cow == "I":
            Is += 1

    if Is == 1:
        print 1
    elif Is>1:
        print 0
    else:
        print As

if __name__ == '__main__':
    main()