# coding=utf-8
__author__ = 'André Meneghelli'

"""
/*******************************************************************************
 * Aluno:  André Meneghelli Vale,      Núm. USP: 4898948
 * Curso: Bacharelado em Ciências da Computação
 * Aula 16 - Sightseeing Trip
 * MAC0327 -- IME/USP, -- Prof. Cristina Gomes Fernandes
 ******************************************************************************/
"""

import sys

pontos = set()


def run_2_the_hills(pos, mapa, origem, destino):
    global pontos
    if pos in pontos:
        pontos.remove(pos)
    if origem != pos and pos == destino:
        return {'caminho': [], 'custo': 0}
    for estrada in mapa[pos]:
        if estrada[0] == origem:
            continue
        r = run_2_the_hills(estrada[0], mapa, pos, destino)
        if r is not None:
            return {'caminho': [pos] + r['caminho'], 'custo': estrada[1] + r['custo'] }
    return None

def main():
    all_input = iter(sys.stdin.read().strip().replace('\n', ' ').split(' '))
    while True:
        global pontos
        top = -1
        mapa = []
        for i in range(100):
            mapa.append([])
        pontos = set()
        n = int(all_input.next())
        if n == -1:
            break
        m = int(all_input.next())
        for _ in xrange(m):
            a = int(all_input.next())-1
            b = int(all_input.next())-1
            c = int(all_input.next())
            if top < a:
                top = a
            if top < b:
                top = b
            pontos.add(a)
            pontos.add(b)
            mapa[a].append((b,c))
            mapa[b].append((a,c))
        for i in range(n):
            mapa[i].sort(key=lambda tup: tup[1])

        caminho = []
        v = None
        for i in range(n):
            if i in pontos:
                pos = i
                origem = i
                caminho.append(i)
                while True:
                    if origem != pos and pos == i:
                        break
                    for estrada in mapa[pos]:
                        if estrada[0] == i:
                            continue
                        r = run_2_the_hills(estrada[0], mapa, pos, destino)
                        if r is not None:
                            return {'caminho': [pos] + r['caminho'], 'custo': estrada[1] + r['custo'] }

                if pos in pontos:
                    pontos.remove(pos)
                if origem != pos and pos == destino:
                    return {'caminho': [], 'custo': 0}
                return None






                vatu = run_2_the_hills(i, mapa, i, i)
                if vatu is not None:
                    if v is not None:
                        if vatu['custo'] < v['custo']:
                            v = vatu
                            # pprint(mapa)
                            # print sum(map(lambda x: len(x) > 0, mapa[1]))
                    else:
                        v = vatu
        # print " ".join(map(lambda x: x + 1, v['caminho']))
        if v is not None:
            print " ".join(map(lambda x: str(x + 1), v['caminho']))
        else:
            print "No solution."
    return
if __name__ == '__main__':
    main()