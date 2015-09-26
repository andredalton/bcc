# coding=utf-8
__author__ = 'andre'

"""
/*******************************************************************************
 * Aluno:  André Meneghelli Vale,      Núm. USP: 4898948
 * Curso: Bacharelado em Ciências da Computação
 * Aula 4 - Desafio B
 * MAC0327 -- 13/07/2015 -- IME/USP, -- Prof. Cristina Gomes Fernandes
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

def main():
    k = int(raw_input())
    
    indices = []
    for i in range(k):
        indices.append(int(raw_input()))
    enesimo(max(indices))

    for i in indices:
        print primos[i-1]

if __name__ == "__main__":
    main()