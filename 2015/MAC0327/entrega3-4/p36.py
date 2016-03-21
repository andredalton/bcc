# coding=utf-8
"""
Nome: André Meneghelli Vale
NUSP: 4898948
Problema P36 - A. Cows and Sequence
"""

def main():
    n = int(raw_input())
    sequencia = [0]
    soma = 0.0
    ultimo = 0
    for i in xrange(n):
        op = map(int, raw_input().split())
        if op[0] == 1:
            if op[1] < len(sequencia):
                sequencia[op[1]-1] -= op[2]
            else:
                sequencia[op[1]-1] += op[2]
            soma += op[2] * op[1]
            ultimo = sequencia[-1]
        elif op[0] == 2:
            sequencia[-1] = op[1] - ultimo
            sequencia.append(op[1])
            soma += sequencia[-1]
            ultimo = sequencia[-1]
        else:
            x = sequencia.pop()
            ultimo = (sequencia[-1] - x) * -1
            sequencia[-1] = ultimo
            soma -= x


        print soma / (len(sequencia))

if __name__ == '__main__':
    main()