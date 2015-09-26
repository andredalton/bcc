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

def bin_search(lst, value, left, right):
    if len(lst) == 0:
        return 0
    l = left
    r = right
    m = (l+r)/2
    while r > l:
        # print l, m, r
        if lst[m] < value:
            l = m + 1
        elif lst[m] > value:
            r = m - 1
        else:
            r = m
        m = (r+l)/2
    if value != lst[m]:
        return False
    return m

def main():
    n = int(raw_input())
    results = map(lambda x: x == "1", raw_input().split())
    last = results[len(results)-1]
    ppetya = []
    pgena = []

    petya = gena = 0
    for result in results:
        if result:
            petya += 1
        else:
            gena += 1
        ppetya.append(petya)
        pgena.append(gena)

    maxp = petya if petya > gena else gena

    print range(n)
    print map(lambda x: 1 if x else 2, results)
    print ppetya
    print pgena
    print maxp

    for t in range(2, maxp+1):
        tpetya = tgena = i = 0
        while i < len(results):
            left = i+t-1
            right = i+2*t-2 if i+2*t-2 < len(results) else len(results)-1
            left = i + 1
            right = len(results)
            print "------------------"
            print "limits:", i, left, right
            gena = bin_search(pgena, tgena + t, 0, len(results)-1)
            petya = bin_search(ppetya, tpetya + t, 0, len(results)-1)
            i = 1 + (gena if gena < petya and gena is not False else petya)
            print "busca:", petya, gena, i, "\ttargets:", tpetya+t, tgena+t
            print "pos:", ppetya[i], pgena[i]
            tgena = pgena[i]
            tpetya = ppetya[i]

    return True

    for r in results[::-1]:
        sums[r] += 1
        if sums[end] > sums[not end]:
            print "Testando para", sums[end]

    return False
    win = sum(results)
    win = win if results[len(results) - 1] else len(results) - win
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

    for t in range(1, max + 1):
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
        if petya == 0 and gena == 0 and sgena != spetya and (spetya > sgena) != results[len(results) - 1]:
            games.append((spetya if (spetya > sgena) else sgena, t))
    print len(games)
    games.sort()
    for game in games:
        print game[0], game[1]


if __name__ == "__main__":
    main()
