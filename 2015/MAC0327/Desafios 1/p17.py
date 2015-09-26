# coding=utf-8
__author__ = 'andre'

"""
/*******************************************************************************
 * Aluno:  André Meneghelli Vale,      Núm. USP: 4898948
 * Curso: Bacharelado em Ciências da Computação
 * Aula 4 - Desafio C
 * MAC0327 -- 12/08/2015 -- IME/USP, -- Prof. Cristina Gomes Fernandes
 ******************************************************************************/
"""
def bin_search(lst, value):
    l = 0
    r = len(lst)-1
    m = (l+r)/2
    while r > l and (lst[m][0] > value or lst[m][1] < value):
        if lst[m][1] < value:
            l = m + 1
        elif lst[m][0] > value:
            r = m - 1
        m = (r+l)/2
    return m if m >= 0 else 0

def main():
    n = int(raw_input())
    wall = [(0, 1000000000)]

    for i in range(n):
        (start, end, color) = raw_input().split()
        if start == end:
            continue
        start = int(start)
        end = int(end)
        if color == 'w':
            pos = bin_search(wall, start)
            if len(wall) > 0 and wall[pos][1] < start:
                pos += 1
            if pos < len(wall) and wall[pos][0] < start:
                start = wall[pos][0]

            while pos < len(wall) and wall[pos][0] <= end:
                if wall[pos][1] >= end:
                    end = wall[pos][1]
                del wall[pos]
            wall.insert(pos, (start, end))
        else:
            pos = bin_search(wall, start)
            if len(wall) > 0 and wall[pos][1] < start:
                pos += 1
            iend = False
            if pos < len(wall) and wall[pos][0] <= start:
                start = (wall[pos][0], start)
                if wall[pos][1] >= end:
                    iend = (end, wall[pos][1])
                del wall[pos]
                wall.insert(pos, start)
                pos += 1
            while iend is False and pos < len(wall) and wall[pos][0] <= end:
                if wall[pos][1] >= end:
                    iend = (end, wall[pos][1])
                del wall[pos]
            if iend is not False:
                wall.insert(pos, iend)

    mw = (0, (0, 0))
    for i in range(len(wall)):
        tam = wall[i][1] - wall[i][0]
        if tam > mw[0]:
            mw = (tam, wall[i])
    print mw[1][0], mw[1][1]

if __name__ == "__main__":
    main()