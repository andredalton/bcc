# coding=utf-8
__author__ = 'andre'

"""
/*******************************************************************************
 * Aluno:  André Meneghelli Vale,      Núm. USP: 4898948
 * Curso: Bacharelado em Ciências da Computação
 * Aula 4 - Desafio C
 * MAC0327 -- 13/07/2015 -- IME/USP, -- Prof. Cristina Gomes Fernandes
 ******************************************************************************/
"""

if __name__ == "__main__":
    n = int(raw_input())
    somas = {}
    for i in range(10**(n/2)):
        aux = i
        soma = 0
        while aux>0:
            soma += aux%10
            aux /= 10
        try:
            somas[soma] += 1
        except KeyError:
            somas[soma] = 1
    print sum(map(lambda x: somas[x]**2, somas))