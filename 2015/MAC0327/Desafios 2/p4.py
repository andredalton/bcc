# coding=utf-8
__author__ = 'André Meneghelli'

"""
/*******************************************************************************
 * Aluno:  André Meneghelli Vale,      Núm. USP: 4898948
 * Curso: Bacharelado em Ciências da Computação
 * Aula 11 - Taxi
 * MAC0327 -- IME/USP, -- Prof. Cristina Gomes Fernandes
 ******************************************************************************/
"""


def main():
    (p, dp, t, dt) = map(int, raw_input().split())

    if p >= t:
        print p
    else:
        while t > p:
            p += dp
            if t <= p:
                print t
            else:
                t -= dt
                if t <= p:
                    print p

if __name__ == '__main__':
    main()