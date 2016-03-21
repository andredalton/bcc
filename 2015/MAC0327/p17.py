# coding=utf-8
"""
Nome: André Meneghelli Vale
NUSP: 4898948
MAC0327 - 2o sem 2015
3a aval
Problema P17 - 1207. Median on the Plane
"""

global embaixo

def compara_angulos(p, q):
    global embaixo
    if p == embaixo:
        return 1
    elif q == embaixo:
        return -1

    x1, y1 = embaixo[0:2]
    x2, y2 = p[0:2]
    x3, y3 = q[0:2]
    res = (x2-x1) * (y3-y1) - (x3-x1) * (y2-y1)
    if res < 0:
        return -1
    elif res > 0:
        return 1
    else:
        return 0

def main():
    global embaixo
    embaixo = []
    pontos = []

    n = int(raw_input())

    for i in xrange(n):
        x, y = map(int, raw_input().split())
        # Armazenando na posição 2 o índice deste ponto
        pontos.append([x, y, i+1])
        if embaixo == [] or y < embaixo[1]:
            embaixo = [x, y, i + 1]

    pontos.sort(compara_angulos)
    # Imprimindo o índice dos pontos requeridos.
    print pontos[n-1][2], pontos[((n-2)/2)][2]

if __name__ == '__main__':
    main()