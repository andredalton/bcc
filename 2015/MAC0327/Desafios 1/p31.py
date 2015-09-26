# coding=utf-8
__author__ = 'andre'

"""
/*******************************************************************************
 * Aluno:  André Meneghelli Vale,      Núm. USP: 4898948
 * Curso: Bacharelado em Ciências da Computação
 * Desafio 31 -  Flags
 * MAC0327 -- 12/08/2015 -- IME/USP, -- Prof. Cristina Gomes Fernandes
 ******************************************************************************/
"""

v = [-1]*46

def F(n):
    global v
    if n == 1:
        return n + 1
    elif n == 0:
        return n
    elif v[n] != -1:
        return v[n]
    else:
        v[n] = F(n-1) + F(n-2)
    return v[n]

print F(int(raw_input()))