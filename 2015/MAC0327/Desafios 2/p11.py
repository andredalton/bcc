# coding=utf-8
__author__ = 'André Meneghelli'

"""
/*******************************************************************************
 * Aluno:  André Meneghelli Vale,      Núm. USP: 4898948
 * Curso: Bacharelado em Ciências da Computação
 * Aula 13 - Stone Pile
 * MAC0327 -- IME/USP, -- Prof. Cristina Gomes Fernandes
 ******************************************************************************/
"""


pedras = []

def procura(s1, s2, index):
    global pedras
    if index == -1:
        return abs(s1-s2)
    sa = procura(s1 + pedras[index], s2, index-1)
    sb = procura(s1, s2 + pedras[index], index-1)
    if sa < sb:
        return sa
    return sb

def main():
    global pedras

    s1 = 0
    raw_input()
    pedras = map(int, raw_input().split())
    pedras.sort()

    s2 = pedras[len(pedras)-1]

    print procura(s1, s2, len(pedras)-2)

if __name__ == '__main__':
    main()
