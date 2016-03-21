# coding=utf-8
"""
Nome: André Meneghelli Vale
NUSP: 4898948
Problema P10 - 1084. Goat in the Garden
"""

__author__ = 'avale'

from math import sqrt, pi, acos, sin

def main():
    (l, r) = map(float, raw_input().split())

    # Circulo menor que o quadrado
    if l/2 >= r:
        print round(pi*r**2, 3)
    # Quadrado dentro do circulo
    elif sqrt(2)*l/2 <= r:
        print l**2
    # Quadrado recortando circulo
    else:
        theta = acos(l/2/r)
        triangulo = r**2*sin(2*theta)/2
        setor = theta*r**2
        borda = setor - triangulo
        print round(pi*r**2 - 4*borda, 3)
if __name__ == '__main__':
    main()
