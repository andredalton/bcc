__author__ = 'avale'

import sys
from random import randint


def main():
    if len(sys.argv) < 3:
        return False
    n = int(sys.argv[1])
    m = int(sys.argv[2])
    usados = set()
    linhas = []

    for i in range(n):
        linhas.append([m, i+2, 1])
        usados.add((i+2, 1))
        while len(linhas[i]) < m + 1:
            a = linhas[i][len(linhas[i])-1]
            b = randint(2, 10000)
            if (a, b) not in usados:
                linhas[i].append(b)
                usados.add((a,b))
        linhas[i].append(i+2)

    print n
    for linha in linhas:
        print " ".join(map(str, linha))

if __name__ == '__main__':
    main()
