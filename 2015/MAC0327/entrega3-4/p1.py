# coding=utf-8
"""
Nome: André Meneghelli Vale
NUSP: 4898948
Problema P1 - 1588. Jamaica
"""

import math

def main():
    n = int(raw_input())

    cidades = []
    # Lendo todas as cidades da entrada
    for i in xrange(n):
        x, y = map(int, raw_input().split())
        cidades.append((x, y))

    angulos = {}
    # Percorrendo cidades
    for i in xrange(n):
        x0, y0 = cidades[i]
        # Percorrendo as próximas cidades
        for j in xrange(i+1, n):
            x1, y1 = cidades[j]
            cateto_o = 1.0*(y1-y0)
            cateto_a = 1.0*(x1-x0)

            if y0 == y1:
                tangente = float('inf')
                if x0 < x1:
                    projecao = 10005+y1
                else:
                    projecao = 10005+y0
            elif x0 == x1:
                tangente = 0
                projecao = x0
            else:
                tangente = (cateto_o/cateto_a)
                projecao = (x1*y0 - x0*y1)/(1.0*(y0-y1))
            dist = math.sqrt((x1-x0)*(x1-x0)+(y1-y0)*(y1-y0))
            # Inserindo a nova tupla (tangente, projeção), caso ela já exista, mantém a maior distância.
            if (tangente, projecao) not in angulos:
                angulos[(tangente, projecao)] = dist
            elif angulos[(tangente, projecao)] < dist:
                angulos[(tangente, projecao)] = dist

    estradas = 0
    for a in angulos:
        estradas += angulos[a]

    print "{0:.0f}".format(round(estradas))

if __name__ == '__main__':
    main()