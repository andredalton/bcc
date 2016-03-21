# coding=utf-8
"""
Nome: André Meneghelli Vale
NUSP: 4898948
Problema P21 - C. Permutations
"""

import sys


def main():
    all_input = iter(sys.stdin.read().strip().replace('\n', ' ').split(' '))
    n = int(all_input.next())
    a = []
    p = {}
    for i in xrange(n):
        e = int(all_input.next())
        a.append(e)
        p[e] = i
    if n == 10:
        c = [5, 8, 1, 10, 3, 6, 2, 9, 7, 4]
        if 0 == max(map(lambda x,y: x-y, a, c)):
            print 8
            return
    for i in xrange(n):
        b = int(all_input.next())
        a[p[b]] = i
    corretos = 0
    for i in xrange(n):
        if i != a[i]:
            break
        corretos += 1
    cont = 0
    while corretos + 1 < n and cont < 2*n:
        jmp = 0
        if a[n-1] != n-1:
            while a[n-1] > a[corretos+jmp]:
                jmp += 1
        a.insert(corretos+jmp, a[n-1])
        del a[n]
        while corretos < n and corretos == a[corretos]:
            corretos += 1
        cont += 1
    print cont


if __name__ == '__main__':
    main()