#! /usr/bin/env python3

from kind import blank, bug, dragon, eletric, fighting, fire, flying, ghost, grass, ground, ice, normal, poison, psychic, rock, water
from skill.attack import Attack

class Pokemon():
    """ Pokemon."""
    def __init__(self):
        self.name = ""
        self.classe = ""
        self.level = 0
        self.HP = 0
        self.ATK = 0
        self.DEF = 0
        self.SPD = 0
        self.SPC = 0
        self.kind1 = blank
        self.kind2 = blank
        self.action1 = None
        self.action2 = None
        self.action3 = None
        self.action4 = None
        self.nattack = 0

    def get_name(self):
        return self.name

    def get_level(self):
        return self.level

    def get_HP(self):
        return self.HP

    def get_ATK(self):
        return self.ATK
    
    def get_DEF(self):
        return self.DEF

    def get_SPC(self):
        return self.SPC

    def get_SPD(self):
        return self.SPD

    def get_kind1(self):
        return self.kind1
    
    def get_kind2(self):
        return self.kind2

    def get_action1(self):
        return self.action1

    def get_action2(self):
        return self.action2

    def get_action3(self):
        return self.action3

    def get_action4(self):
        return self.action4

    def load_file(self,name):
        fo = open(name, "r")
        self.name = fo.readline().strip()
        self.level = int(fo.readline())
        self.HP = int(fo.readline())
        self.ATK = int(fo.readline())
        self.DEF = int(fo.readline())
        self.SPD = int(fo.readline())
        self.SPC = int(fo.readline())
        try:
            self.kind1 = globals()[fo.readline().strip().lower()]
        except KeyError:
            self.kind1 = globals()["blank"]
        try:
            self.kind2 = globals()[fo.readline().strip().lower()]
        except KeyError:
            self.kind2 = globals()["blank"]
        n = self.SPC = int(fo.readline())
        if n > 4:
            n = 4
        for i in range(n):
            nameh = fo.readline().strip()
            try:
                tp = globals()[fo.readline().strip().lower()]
            except KeyError:
                tp = globals()["blank"]
            acc = float(fo.readline())
            power = int(fo.readline())
            pp = int(fo.readline())

            self.__dict__["action"+str(i+1)] = Attack(nameh, tp, acc, power, pp, self)
            self.nattack += 1
        fo.close()

    def print_attack(self):
        for i in range(4):
            a = self.__dict__["action"+str(i+1)]
            if a is None:
                break
            params = {"n":i+1, "name":a.get_name(), "pp":a.get_pp(), "ppm":a.get_ppm()}

            if a is not None:
                print("%(n)d - %(name)s (%(pp)d/%(ppm)d)" % params)

    def get_damage(self, dano):
        if self.HP < dano:
            self.HP = 0
        else:
            self.HP -= int(dano)

    def get_nattack(self):
        return self.nattack
    
if __name__ == '__main__':
    pass