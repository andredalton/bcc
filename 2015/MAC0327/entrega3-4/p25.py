# coding=utf-8
"""
Nome: André Meneghelli Vale
NUSP: 4898948
Problema P25 - B. Young Photographer
"""

import sys

def main():
    all_input = iter(sys.stdin.read().strip().replace('\n', ' ').split(' '))
    n = int(all_input.next())
    x0 = int(all_input.next())
    road = [0]*1001

    for i in xrange(n):
        a = int(all_input.next())
        b = int(all_input.next())
        if a > b:
            c = a
            a = b
            b = c
        for j in xrange(a, b+1):
            road[j] += 1
    try:
        pi = road.index(n)
        pf = (len(road) - 1) - road[::-1].index(n)
    except ValueError:
        print -1
        return

    if x0 <= pi:
        print pi-x0
    elif x0 <= pf:
        print 0
    else:
        print x0-pf

if __name__ == '__main__':
    main()
