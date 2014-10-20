#! /usr/bin/env python3

from kind import blank, bug, dragon, eletric, fighting, fire, flying, ghost, grass, ground, ice, normal, poison, psychic, rock, water
from skill.attack import Attack
from skill.counter import Counter
from skill.struggle import Struggle

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
        self.struggle = Struggle( globals()["normal"], self)
        self.last_hit = 0
        self.nhits = 0

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

    def left_pp(self):
        pp = 0
        if self.action1 is not None:
            pp += self.action1.get_pp()
        if self.action2 is not None:
            pp += self.action2.get_pp()
        if self.action3 is not None:
            pp += self.action3.get_pp()
        if self.action4 is not None:
            pp += self.action4.get_pp()
        return pp

    def select_attack(self, n):
        if n == 1 and self.action1 is not None:
            if self.action1.get_pp() > 0:
                return self.action1
            else:
                print("No PP left for this move!")
                return None
        if n == 2  and self.action2 is not None:
            if self.action2.get_pp() > 0:
                return self.action2
            else:
                print("No PP left for this move!")
                return None
        if n == 3 and self.action3 is not None:
            if self.action3.get_pp() > 0:
                return self.action3
            else:
                print("No PP left for this move!")
                return None
        if n == 4 and self.action4 is not None:
            if self.action4.get_pp() > 0:
                return self.action4
            else:
                print("No PP left for this move!")
                return None
        if n == "struggle":
            return self.struggle
        return None

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

            if nameh == "Counter":
                self.__dict__["action"+str(i+1)] = Counter(tp, pp, self)
            else:
                self.__dict__["action"+str(i+1)] = Attack(nameh, tp, acc, power, pp, self)
            self.nattack += 1
        fo.close()

    def print_attack(self):
        if self.left_pp() > 0:
            for i in range(4):
                a = self.__dict__["action"+str(i+1)]
                if a is None:
                    break
                params = {"n":i+1, "name":a.get_name(), "pp":a.get_pp(), "ppm":a.get_ppm()}

                if a is not None:
                    print("%(n)d - %(name)s (%(pp)d/%(ppm)d)" % params)
            return True
        return False

    def get_damage(self, dano):
        if self.HP < dano:
            self.HP = 0
        else:
            self.HP -= int(dano)
        self.last_hit = dano
        self.nhits += 1

    def get_nattack(self):
        return self.nattack

    def get_last_hit(self):
        return self.last_hit

    def get_nhits(self):
        return self.nhits
    
if __name__ == '__main__':
    pass