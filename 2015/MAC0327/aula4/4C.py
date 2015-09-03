# coding=utf-8
__author__ = 'andre'

"""
/*******************************************************************************
 * Aluno:  André Meneghelli Vale,      Núm. USP: 4898948
 * Curso: Bacharelado em Ciências da Computação
 * Aula 4 - Desafio C
 * MAC0327 -- 13/07/2015 -- IME/USP, -- Prof. Cristina Gomes Fernandes
 ******************************************************************************/
"""
def bin_search(lst, value):
    l = 0
    r = len(lst)-1
    m = (l+r)/2
    while r > l:
        # print l, m, r
        if lst[m] < value:
            l = m + 1
        elif lst[m] > value:
            r = m - 1
        else:
            r = m
        m = (r+l)/2
    if value != lst[m]:
        return False
    return m

def main():
    n = int(raw_input())
    wall = []

    for i in range(n):
        (start, end, color) = raw_input().split()
        if color == 'w':
            wall.append((int(start), int(end)))
            print "Pintando de branco:", start, end
        else:
            print "Pintando de preto:", start, end

if __name__ == "__main__":
    main()