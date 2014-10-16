#! /usr/bin/env python3

import sys
import os

sys.path.append(os.path.abspath(os.path.join(os.path.dirname(__file__), 'tipo')))
import tipo
import water
import fire

sys.path.append(os.path.abspath(os.path.join(os.path.dirname(__file__), 'habilidade')))
import habilidade

class Pokemon():
    """ Pokemon."""
    def __init__(self):
        self.nome = ""
        self.classe = ""
        self.nivel = 0
        self.HP = 0
        self.ATK = 0
        self.DEF = 0
        self.SPD = 0
        self.SPC = 0
        self.tipo1 = None
        self.tipo2 = None #fire.Fire()
        self.acao1 = None #habilidade.Habilidade()
        self.acao2 = None
        self.acao3 = None
        self.acao4 = None

    def set_tipo(self, tipo):
        pass

    def get_nivel(self):
        return self.nivel

    def get_ATK(self):
        return self.ATK
    
    def get_DEF(self):
        return self.DEF

    def get_tipo1(self):
        return self.tipo1
    
    def get_tipo2(self):
        return self.tipo2

    def carrega_arquivo(self,nome):
        fo = open(nome, "r")
        self.nome = fo.readline()
        self.nivel = int(fo.readline())
        self.HP = int(fo.readline())
        self.ATK = int(fo.readline())
        self.DEF = int(fo.readline())
        self.SPD = int(fo.readline())
        self.SPC = int(fo.readline())
        tipo1 = fo.readline()
        tipo2 = fo.readline()
        n = self.SPC = int(fo.readline())
        for i in range(n):
            nome = fo.readline()
            tipo = fo.readline()
            acuracia = fo.readline()
            power = fo.readline()
            pp = fo.readline()

    def recebe_dano(self, dano):
        self.HP -= dano
        print(self.tipo1.get_fraqueza("Water"))
        print(self.tipo2.get_fraqueza("Water"))

    
if __name__ == '__main__':
    p = Pokemon()
    p.carrega_arquivo("billpc/ratata")
    print(p.get_nivel())