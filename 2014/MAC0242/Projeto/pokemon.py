#! /usr/bin/env python3

import tipo
import acao

class Pokemon():
    """ Pokemon."""
    def __init__(self):
        self.nome = ""
        self.nivel = 0
        self.HP = 0
        self.ATK = 0
        self.DEF = 0
        self.SPD = 0
        self.SPC = 0
        self.tipo1 = tipo.Tipo()
        self.tipo2 = tipo.Tipo()
        self.acao1 = acao.Acao()
        self.acao2 = acao.Acao()
        self.acao3 = acao.Acao()
        self.acao4 = acao.Acao()

if __name__ == '__main__':
    pass