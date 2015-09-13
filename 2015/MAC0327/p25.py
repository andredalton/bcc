# coding=utf-8
__author__ = 'andre'

"""
/*******************************************************************************
 * Aluno:  André Meneghelli Vale,      Núm. USP: 4898948
 * Curso: Bacharelado em Ciências da Computação
 * Desafio 25 -  Brave Balloonists
 * MAC0327 -- 12/08/2015 -- IME/USP, -- Prof. Cristina Gomes Fernandes
 ******************************************************************************/
"""

primos = [2]

def primao(primo):
    for p in primos:
        if primo%p == 0:
            return False
        if p**2 > primo:
            break
    return True

def enesimo(n):
    p = 3
    for i in range(1, n):
        while i == len(primos):
            if primao(p):
                primos.append(p)
            p += 2

def all_factors(n):
    global primos
    remainder = n
    factors = {}
    index = 0
    divider = 2
    while remainder > 1:
        if remainder % divider == 0:
            if divider in factors:
                factors[divider] += 1
            else:
                factors[divider] = 1
            remainder /= divider
        else:
            index += 1
            divider = primos[index]
    return factors

def main():
    global primos
    enesimo(1229)

    mfact = []
    for i in range(10):
        n = int(raw_input())
        mfact.append(all_factors(n))

    factors = {}
    for m in mfact:
        for f, v in m.iteritems():
            try:
                factors[f] += v
            except KeyError:
                factors[f] = v

    print reduce(lambda x, y: x*y, map(lambda x: x+1, factors.values())) % 10
    return True

if __name__ == "__main__":
    main()
