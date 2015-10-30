# coding=utf-8
__author__ = 'André Meneghelli'

"""
/*******************************************************************************
 * Aluno:  André Meneghelli Vale,      Núm. USP: 4898948
 * Curso: Bacharelado em Ciências da Computação
 * Aula 16 - Map Coloring
 * MAC0327 -- IME/USP, -- Prof. Cristina Gomes Fernandes
 ******************************************************************************/
"""


mapa = {}
cores = None
sem_pintura = []


def pinta(cor, indice):
    global cores
    global mapa
    global sem_pintura
    if cores[indice] == (cor+1)%2:
        return False
    if cores[indice] == -1:
        cores[indice] = cor
        sem_pintura.remove(indice)
        for i in mapa[indice]:
            if pinta((cor+1)%2, i) is False:
                return False
    return True


def main():
    global cores
    global sem_pintura
    global mapa

    n = int(raw_input())
    if n == 0:
        return False

    cores = [-1] * (n + 1)

    for i in range(1, n+1):
        fronteiras = map(int, raw_input().split())
        if mapa.has_key(i):
            mapa[i] += fronteiras[:len(fronteiras)-1]
        else:
            mapa[i] = fronteiras[:len(fronteiras)-1]
        for f in fronteiras[:len(fronteiras)-1]:
            if mapa.has_key(f):
                mapa[f] += [i]
            else:
                mapa[f] = [i]
        sem_pintura.append(i)

    while len(sem_pintura)>0:
        if pinta(0, sem_pintura[0]) is False:
            return False
    return True


if __name__ == '__main__':
    if main():
        print "".join(map(str, cores[1:]))
    else:
        print -1
