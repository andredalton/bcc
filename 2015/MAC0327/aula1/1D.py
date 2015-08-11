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

def main():
    raw_input()
    results = map(lambda x: x == "1", raw_input().split())
    max = sum(results)
    max = max if 2*max >= len(results) else len(results) - max
    games = []

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

