# coding=utf-8
__author__ = 'andre'

"""
/*******************************************************************************
 * Aluno:  André Meneghelli Vale,      Núm. USP: 4898948
 * Curso: Bacharelado em Ciências da Computação
 * Aula 4 - Desafio C
 * MAC0327 -- 13/07/2015 -- IME/USP, -- Prof. Cristina Gomes Fernandes
 ******************************************************************************/

Esse meu jeito de fuder
Ninguém nunca fode igual

A minha vida é fazer
Um cu sentar no pau

Pelo mundo viajarei,
tentando masturbar

Um pokemon com o seu poder
e em tudo esporrar

Temos que gozar! Pokemon
"""

class White_node:
    def __init__(self, limits):
        self.limits = limits
        self.left = None
        self.right = None

    def __str__(self):
        return str(self.limits)

    def add(self, node):
        if node < self:
            if self.left is not None:
                self.left.add(node)
            else:
                self.left = node
        elif node > self:
            if self.right is not None:
                self.right.add(node)
            else:
                self.right = node
        else:
            if node[0] < self.limits[0]:
                pass


    def remove(self, node):
        pass

    def __gt__(self, other):
        if other is None:
            return True
        return other.limits[1] < self.limits[0]

    def __lt__(self, other):
        if other is None:
            return True
        return other.limits[0] > self.limits[1]

    def __eq__(self, other):
        if other is None:
            return False
        c1 = other.limits[0] <= self.limits[1] and other.limits[0] >= self.limits[0]
        c2 = other.limits[1] <= self.limits[1] and other.limits[1] >= self.limits[0]
        return c1 or c2

def main():
    n = int(raw_input())
    head == None
    for i in range(n):
        (start, end, color) = raw_input().split()
        if head is None:
            if color == "b":
                continue
            head = White_node((int(start), int(end)))
        else:
            if color == "w":
                head.add(White_node((int(start), int(end))))
            else:
                head.remove(White_node((int(start), int(end))))

if __name__ == "__main__":
    main()