# coding=utf-8
__author__ = 'André Meneghelli'

"""
/*******************************************************************************
 * Aluno:  André Meneghelli Vale,      Núm. USP: 4898948
 * Curso: Bacharelado em Ciências da Computação
 * Aula 14 - Anansi's Cobweb
 * MAC0327 -- IME/USP, -- Prof. Cristina Gomes Fernandes
 ******************************************************************************/
"""


import sys

nrotulos = 0

class Rotulo:
    def __init__(self, i):
        self.elementos = {i}
        self.__index = i
        self.parent = self

    def append(self, r):
        global nrotulos
        nrotulos -= 1
        self.elementos.union(r.elementos)
        r.parent = self

    def index(self):
        r = self.parent
        while r.parent != r:
            r = r.parent
        return r.__index

def main():
    global nrotulos
    all_input = iter(sys.stdin.read().strip().replace('\n', ' ').split(' '))
    n = int(all_input.next())
    nrotulos = n
    componentes = map(lambda x: Rotulo(x), range(n))

    buff = []

    m = int(all_input.next())
    for _ in range(m):
        a = int(all_input.next()) - 1
        b = int(all_input.next()) - 1
        buff.append((a,b))

    destruidos = []
    q = int(all_input.next())

    for _ in range(q):
        d = int(all_input.next())
        destruidos.append(d-1)

    for i in range(m):
        if i not in destruidos:
            a, b = buff[i]
            if componentes[a].index() != componentes[b].index():
                componentes[a].append(componentes[b])

    buffcomponents = [nrotulos]

    for des in reversed(destruidos[1:]):
        a, b = buff[des]
        if nrotulos > 1:
            if componentes[a].index() != componentes[b].index():
                componentes[a].append(componentes[b])
        buffcomponents.append(nrotulos)

    for i in xrange(q):
        print buffcomponents[q-i-1],

if __name__ == '__main__':
    main()