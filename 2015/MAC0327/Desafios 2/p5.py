# coding=utf-8
__author__ = 'André Meneghelli'

"""
/*******************************************************************************
 * Aluno:  André Meneghelli Vale,      Núm. USP: 4898948
 * Curso: Bacharelado em Ciências da Computação
 * Aula 12 - Turn for MEGA
 * MAC0327 -- IME/USP, -- Prof. Cristina Gomes Fernandes
 ******************************************************************************/
"""


def main():
    (v, t) = map(int, raw_input().split())
    e = map(int, raw_input().split())

    fila = 0

    for s in e:
        fila += s
        fila -= v
        if fila < 0:
            fila = 0

    print fila


if __name__ == '__main__':
    main()