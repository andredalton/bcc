# coding=utf-8
__author__ = 'André Meneghelli'

"""
/*******************************************************************************
 * Aluno:  André Meneghelli Vale,      Núm. USP: 4898948
 * Curso: Bacharelado em Ciências da Computação
 * Aula 12 - Husband in a Shop
 * MAC0327 -- IME/USP, -- Prof. Cristina Gomes Fernandes
 ******************************************************************************/
"""


def main():
    m = int(raw_input())

    market = {}
    for i in range(m):
        (qt, name, name) = raw_input().split()
        market[name] = int(qt)

    n = int(raw_input())
    queue = []
    for i in range(n):
        (qt, name, name) = raw_input().split()
        queue.append({'qt': int(qt), 'name': name})

    queue.append({'qt': 'me'})

    time = 0
    while len(queue) > 0:
        guy = queue.pop(0)
        if guy['qt'] == 'me':
            break
        if market.has_key(guy['name']):
            if market[guy['name']] > 0:
                if market[guy['name']] >= guy['qt']:
                    market[guy['name']] -= guy['qt']
                else:
                    guy['qt'] = market[guy['name']]
                    queue.insert(1, guy)
        time += 1

    print time


if __name__ == '__main__':
    main()