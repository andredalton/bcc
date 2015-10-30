# coding=utf-8
__author__ = 'André Meneghelli'

"""
/*******************************************************************************
 * Aluno:  André Meneghelli Vale,      Núm. USP: 4898948
 * Curso: Bacharelado em Ciências da Computação
 * Aula 18 - Magnetic Storms
 * MAC0327 -- IME/USP, -- Prof. Cristina Gomes Fernandes
 ******************************************************************************/
"""


def main():
    m = int(raw_input())
    q = []
    maxQ = []

    while True:
        n = int(raw_input())
        if n == -1:
            break
        q.append(n)
        while len(maxQ) > 0 and maxQ[-1] < n:
            del maxQ[-1]
        maxQ.append(n)
        if len(q) == m:
            print maxQ[0]
            if maxQ[0] == q[0]:
                del maxQ[0]
            del q[0]

if __name__ == '__main__':
    main()
