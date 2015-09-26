# coding=utf-8
__author__ = 'andre'

"""
/*******************************************************************************
 * Aluno:  André Meneghelli Vale,      Núm. USP: 4898948
 * Curso: Bacharelado em Ciências da Computação
 * Aula 2 - Desafio B
 * MAC0327 -- 13/07/2015 -- IME/USP, -- Prof. Cristina Gomes Fernandes
 ******************************************************************************/
"""

def check(x, z, t):
    for s, f in z:
        for i in range(s, f+1):
            try:
                if x[i+t]:
                    return True
            except IndexError:
                break
    return False

def main():
    (p, q, l, r) = map(int, raw_input().split())
    x = [False]*1001
    z = []

    for i in range(p):
        (s, e) =  map(int, raw_input().split())
        for t in range(s, e+1):
            x[t] = True
    for i in range(q):
        z.append(map(int, raw_input().split()))

    h = 0
    for i in range(l, r+1):
        if check(x, z, i):
            h += 1
    print h

if __name__ == "__main__":
    main()