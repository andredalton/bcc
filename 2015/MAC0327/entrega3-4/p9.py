# coding=utf-8
"""
Nome: André Meneghelli Vale
NUSP: 4898948
Problema P9 - 1020. Rope
"""

__author__ = 'avale'

from math import pi, sqrt

def main():
    (n, r) = raw_input().split()
    n = int(n)
    r = float(r)

    perimetro = 2*pi*r

    pini = p0 = map(float, raw_input().split())
    for i in range(n-1):
        p1 = map(float, raw_input().split())
        perimetro += sqrt((p0[0]-p1[0])**2+(p0[1]-p1[1])**2)
        p0 = p1
    perimetro += sqrt((p0[0]-pini[0])**2+(p0[1]-pini[1])**2)
    print round(perimetro, 2)

if __name__ == '__main__':
    main()
