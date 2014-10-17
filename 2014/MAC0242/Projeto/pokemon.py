#! /usr/bin/env python3

from tipo import blank, bug, dragon, eletric, fighting, fire, flying, ghost, grass, ground, ice, normal, poison, psychic, rock, water
from habilidade import get_habilidade, add_habilidade, ataque

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
        self.tipo1 = blank
        self.tipo2 = blank
        self.acao1 = None
        self.acao2 = None
        self.acao3 = None
        self.acao4 = None

    def set_tipo(self, tipo):
        pass

    def get_nivel(self):
        return self.nivel

    def get_HP(self):
        return self.HP

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
        self.tipo1 = globals()[fo.readline().strip().lower()]
        self.tipo2 = globals()[fo.readline().strip().lower()]
        n = self.SPC = int(fo.readline())
        for i in range(n):
            nomeh = fo.readline().strip()
            tipo = globals()[fo.readline().strip().lower()]
            acuracia = float(fo.readline())
            power = int(fo.readline())
            pp = int(fo.readline())

            hab = get_habilidade(nomeh)
            if hab is None:
                hab = add_habilidade(ataque.Ataque(nomeh, tipo, acuracia, power, pp))
            self.__dict__["acao"+str(i+1)] = hab
        fo.close()

    def recebe_dano(self, dano):
        self.HP -= dano
        print(self.tipo1.get_fraqueza("Water"))
        print(self.tipo2.get_fraqueza("Water"))
    
if __name__ == '__main__':
    p = Pokemon()
    b = Pokemon()
    p.carrega_arquivo("billpc/ratata")
    b.carrega_arquivo("billpc/ratata")
    
    print("p x b")
    print(p)
    print(b)
    print("acao1")
    print(p.acao1, " ", p.acao1.get_nome())
    print(b.acao1, " ", b.acao1.get_nome())
    print("acao2")
    print(p.acao2, " ", p.acao2.get_nome())
    print(b.acao2, " ", b.acao2.get_nome())
    print("tipo1")
    print(p.tipo1, " ", p.tipo1.get_nome())
    print(b.tipo1, " ", b.tipo1.get_nome())
    print("tipo2")
    print(p.tipo2, " ", p.tipo2.get_nome())
    print(b.tipo2, " ", b.tipo2.get_nome())
    print("acao1.typ")
    print(p.acao1.TYP, " ", p.acao1.TYP.get_nome())
    print(b.acao1.TYP, " ", b.acao1.TYP.get_nome())
    print("acao1.typ")
    print(p.acao2.TYP, " ", p.acao2.TYP.get_nome())
    print(b.acao2.TYP, " ", b.acao2.TYP.get_nome())
