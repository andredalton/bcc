# coding=utf-8
__author__ = 'andre'

"""
/*******************************************************************************
 * Aluno:  André Meneghelli Vale,      Núm. USP: 4898948
 * Curso: Bacharelado em Ciências da Computação
 * Aula 1 - Desafio D
 * MAC0327 -- 10/07/2015 -- IME/USP, -- Prof. Cristina Gomes Fernandes
 ******************************************************************************/
"""

from itertools import combinations, permutations, chain

def all_factors(n):
    remainder = n
    factors = {}
    divider = 2
    while remainder > 1:
        if remainder % divider == 0:
            if divider in factors:
                factors[divider] += 1
            else:
                factors[divider] = 1
            remainder /= divider
        else:
            divider += 1
    return factors

def all_dividers(factors):
    if len(factors) == 1:
        for i in range(factors[0][1]+1):
            yield factors[0][0]**i
    else:
        for i in range(factors[0][1]+1):
            for item in all_dividers(factors[1:]):
                yield item*factors[0][0]**i

def main():
    raw_input()
    results = map(lambda x: x == "1", raw_input().split())
    win = sum(results)
    win = win if results[len(results)-1] else len(results) - win
    games = []

    print win
    factors = all_factors(win)
    dividers = []
    for item in all_dividers(factors.items()):
        dividers.append(item)
    dividers.sort()
    print factors
    print len(dividers)
    print dividers
    # for i in range(5+1):
    #     for j in range(4+1):
    #         print (2**i)*(5**j)

    # print factors
    return True

    for t in range(1, max+1):
        gena = petya = sgena = spetya = 0
        for result in results:
            if result:
                gena += 1
            else:
                petya += 1
            if gena == t:
                sgena += 1
                gena = petya = 0
            if petya == t:
                spetya += 1
                gena = petya = 0
        if petya == 0 and gena == 0 and sgena != spetya and (spetya > sgena) != results[len(results)-1]:
            games.append((spetya if (spetya>sgena) else sgena, t))
    print len(games)
    games.sort()
    for game in games:
        print game[0], game[1]

if __name__ == "__main__":
    main()

