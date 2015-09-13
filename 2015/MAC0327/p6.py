# coding=utf-8
__author__ = 'andre'

"""
/*******************************************************************************
 * Aluno:  André Meneghelli Vale,      Núm. USP: 4898948
 * Curso: Bacharelado em Ciências da Computação
 * Aula 2 - Desafio A
 * MAC0327 -- 08/07/2015 -- IME/USP, -- Prof. Cristina Gomes Fernandes
 ******************************************************************************/
"""

def main():
    levels = int(raw_input())
    xplayer = map(int, raw_input().split())
    yplayer = map(int, raw_input().split())
    xlevels = xplayer[1:]
    ylevels = yplayer[1:]
    wins = set(xlevels+ylevels)

    if len(wins) == levels and sum(wins) == (levels+1)*levels/2:
        print "I become the guy."
    else:
        print "Oh, my keyboard!"

if __name__ == "__main__":
    main()
