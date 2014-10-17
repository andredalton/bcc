#! /usr/bin/env python3

from pokemon import Pokemon

if __name__ == '__main__':
    print("Bem vindo a maior batalha pokemon de todos os tempos!")
    p1 = Pokemon()
    p2 = Pokemon()
    p1.carrega_arquivo("billpc/chucknorris")
    p2.carrega_arquivo("billpc/ratata")
    p1.print_ataque()
    p1.acao1.acao(p1, p2)
    p1.acao2.acao(p1, p2)
    p1.acao1.acao(p1, p2)
    p1.acao1.acao(p1, p2)
    p1.acao1.acao(p1, p2)
    p1.print_ataque()
    