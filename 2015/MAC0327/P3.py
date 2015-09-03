# coding=utf-8
__author__ = 'André Meneghelli'

"""
/*******************************************************************************
 * Aluno:  André Meneghelli Vale,      Núm. USP: 4898948
 * Curso: Bacharelado em Ciências da Computação
 * Aula 1 - Desafio C
 * MAC0327 -- 08/07/2015 -- IME/USP, -- Prof. Cristina Gomes Fernandes
 ******************************************************************************/
"""

SIZE = 100


def line_cmp(lin, col, mtz, deleted, used):
    for i in range(col, len(mtz[lin])):
        if deleted[i]:
            continue
        if mtz[lin+1][i] < mtz[lin][i]:
            deleted[i] = True
            for lin2 in used[i]:
                res = line_cmp(lin2, i+1, mtz, deleted, used)
                if res is not True:
                    used[res].append(lin2)
        elif mtz[lin+1][i] > mtz[lin][i]:
            return i
    return True

def main():
    lins, cols = map(int, raw_input().split())
    mtz = []
    used = []
    deleted = []

    for i in range(lins):
        mtz.append(raw_input())

    for i in range(cols):
        used.append([])
        deleted.append(False)

    for i in range(lins-1):
        res = line_cmp(i, 0, mtz, deleted, used)
        if res is not True:
            used[res].append(i)

    print sum(deleted)

if __name__ == "__main__":
    main()
