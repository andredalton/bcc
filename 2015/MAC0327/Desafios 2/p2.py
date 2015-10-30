# coding=utf-8
__author__ = 'André Meneghelli'

"""
/*******************************************************************************
 * Aluno:  André Meneghelli Vale,      Núm. USP: 4898948
 * Curso: Bacharelado em Ciências da Computação
 * Aula 11 - Knight
 * MAC0327 -- 17/10/2015 -- IME/USP, -- Prof. Cristina Gomes Fernandes
 ******************************************************************************/
"""
board = []

def translate(x, y=False):
    if y is False:
        (x, y) = x
    translate_table = ['a', 'b', 'c', 'd', 'e', 'f', 'g', 'h']
    return translate_table[x] + str(y+1)

def increment_h(i, j):
    global board
    if i >= 0 and i < len(board) and j >= 0 and j < len(board):
        board[i][j]['h'] += 1


def valid_space(i, j):
    global board
    return 0 <= i < len(board) and 0 <= j < len(board) and board[i][j]['free']


def ridin_dirty(x, y, index):
    global board

    board[x][y]['free'] = False
    if index+1 == len(board)**2:
        print board[x][y]['pos']
        return True
    lst = []
    if valid_space(x+2, y+1):
        lst.append(board[x+2][y+1])
    if valid_space(x+2, y-1):
        lst.append(board[x+2][y-1])
    if valid_space(x-2, y+1):
        lst.append(board[x-2][y+1])
    if valid_space(x-2, y-1):
        lst.append(board[x-2][y-1])
    if valid_space(x+1, y+2):
        lst.append(board[x+1][y+2])
    if valid_space(x+1, y-2):
        lst.append(board[x+1][y-2])
    if valid_space(x-1, y+2):
        lst.append(board[x-1][y+2])
    if valid_space(x-1, y-2):
        lst.append(board[x-1][y-2])

    if len(board) != 7: lst = sorted(lst, key=lambda k: k['h'])

    for square in lst:
        if ridin_dirty(square['x'], square['y'], index+1):
            print board[x][y]['pos']
            return True

    board[x][y]['free'] = True
    return False

def main():
    global board
    n = int(raw_input())

    board = [[{'x': y, 'y': x, 'pos': translate(x, y), 'free': True, 'h': 0} for x in range(n)] for y in range(n)]

    for i in range(n):
        for j in range(n):
            increment_h(i+2, j+1)
            increment_h(i+2, j-1)
            increment_h(i-2, j+1)
            increment_h(i-2, j-1)
            increment_h(i+1, j+2)
            increment_h(i+1, j-2)
            increment_h(i-1, j+2)
            increment_h(i-1, j-2)

    if ridin_dirty(0, 0, 0) is False:
        print "IMPOSSIBLE"

    return True

if __name__ == "__main__":
    main()