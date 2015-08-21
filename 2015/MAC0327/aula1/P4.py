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

def bin_search(lst, pos, value):
    print "a"
    if len(lst) == 0:
        return 0
    print "b"
    if value > lst[len(lst)-1][pos]:
        return -1
    print "c"
    l = 0
    r = len(lst)
    m = r/2
    while r-l > 1:
        print "d", r, l, m
        if lst[m][pos] > value:
            r = m
        elif lst[m][pos] < value:
            l = m
        else:
            break
        m = (r+l)/2
        print "e", r, l, m

    print "f", r, l, m
    return m

def main():
    raw_input()
    results = map(lambda x: x == "1", raw_input().split())
    last = results[len(results)-1]
    sums = []

    petya = gena = 0
    for result in results:
        if result:
            petya += 1
        else:
            gena += 1
        sums.append((petya, gena))

    print sums
    print bin_search(sums, 0, 1)
    print bin_search(sums, 1, 1)
    return True

    end = results[len(results)-1]

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
