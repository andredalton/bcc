# coding=utf-8
"""
Nome: André Meneghelli Vale
NUSP: 4898948
Problema P29 - A. Series of Crimes
"""

import sys

def main():
    all_input = iter(sys.stdin.read().strip().replace('\n', ' ').split(' '))
    n = int(all_input.next())
    m = int(all_input.next())

    pts = []
    estado = False
    for i in xrange(n):
        lin = list(all_input.next())
        for j in xrange(m):
            if lin[j] == "*":
                pts.append((i+1, j+1))
        if not estado and len(pts) > 0:
            estado = len(pts)

    if estado == 2:
        print pts[2][0], pts[0][1] if pts[2][1] == pts[1][1] else pts[1][1]
    else:
        print pts[0][0], pts[1][1] if pts[0][1] == pts[2][1] else pts[2][1]

if __name__ == '__main__':
    main()