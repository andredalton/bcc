# Usado pra receber arquivos XML
import xml.etree.ElementTree as ET
from xml.etree.ElementTree import Element, SubElement, Comment, tostring
from xml.etree import ElementTree
from xml.dom import minidom


from .kind import kind_list, kind_dict, blank, bug, dragon, eletric, fighting, flying, fire, ghost, grass, ground, ice, normal, poison, psychic, rock, water

from .attack.attack import Attack
from .attack.struggle import Struggle

class Pokemon():
    """ Pokemon."""
    def __init__(self):
        self.name = ""
        self.level = 0
        self.HP = 0
        self.ATK = 0
        self.DEF = 0
        self.SPD = 0
        self.SPC = 0
        self.kind1 = kind_dict['blank']
        self.kind2 = kind_dict['blank']
        self.attacks = [Struggle(self), None, None, None, None]
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

    def get_attack(self, n):
        return self.attacks[n]

    def left_pp(self):
        pp = 0
        for i in range(1, 5):
            if self.attacks[i] is not None:
                pp += self.attacks[i].get_pp()        
        return pp

    def select_attack(self, n):
        if self.attacks[n] is not None:
            if self.attacks[n].get_pp()>0:
                return self.attacks[n]
            else:
                print("No PP left for this move!")
        return None

    def load_xml(self, root):
        import xml.etree.ElementTree as ET

        print("\npk\n")
        ty  = 1
        nameh = ""
        for pok in root.iter("pokemon"):
            for tp in pok.iter():
                if tp.tag == "attacks":
                    self.nattack += 1
                    if self.nattack > 1:
                        if nameh == "Counter":
                            self.__dict__["action"+idd] = Counter(pp, self)
                        else:
                            self.__dict__["action"+idd] = Attack(nameh, typ, acc, power, pp, self)
                    print()
                    continue
                if self.nattack > 0:
                    print("att>  ", tp.tag, tp.text)
                    if tp.tag == "id":
                        idd = tp.text
                    if tp.tag == "name":
                        nameh = tp.text
                    if tp.tag == "type":
                        try:
                            typ = globals()[tp.text.strip().lower()]
                        except KeyError:
                            typ = globals()["blank"]
                    if tp.tag == "power":
                        power = int(tp.text)
                    if tp.tag == "accuracy":
                        acc = int(tp.text)/100
                    if tp.tag == "power_points":
                        pp = int(tp.text)
                elif tp.tag != "pokemon" and tp.tag != "attributes":
                    if tp.tag == "name":
                        self.name = tp.text
                    if tp.tag == "level":
                        self.level = int(tp.text)
                    if tp.tag == "health":
                        self.HP = int(tp.text)
                    if tp.tag == "attack":
                        self.ATK = int(tp.text)
                    if tp.tag == "defense":
                        self.DEF = int(tp.text)
                    if tp.tag == "speed":
                        self.SPD = int(tp.text)
                    if tp.tag == "special":
                        self.SPC = int(tp.text)
                    if tp.tag == "type":
                        self.__dict__["kind"+str(ty)] = globals()[tp.text.strip().lower()]
                        ty += 1
                    print(tp.tag, tp.text)
            if self.nattack > 0:
                if nameh == "Counter":
                    self.__dict__["action"+idd] = Counter(pp, self)
                else:
                    self.__dict__["action"+idd] = Attack(nameh, typ, acc, power, pp, self)
        pass

    def make_xml(self):
        """ Método que gera um nó XML para este pokemon. """
        pokemon = Element('pokemon')
        name = SubElement(pokemon, 'name')
        name.text = self.name
        level = SubElement(pokemon, 'level')
        level.text = str(self.level)
        attributes = SubElement(pokemon, 'attributes')
        health = SubElement(attributes, 'health')
        health.text = str(self.HP)
        attack = SubElement(attributes, 'attack')
        attack.text = str(self.ATK)
        defense = SubElement(attributes, 'defense')
        defense.text = str(self.DEF)
        speed = SubElement(attributes, 'speed')
        speed.text = str(self.SPD)
        special = SubElement(attributes, 'special')
        special.text = str(self.SPC)
        type1 = SubElement(pokemon, 'type')
        type1.text = str(kind_list.index(self.kind1))
        if self.kind2 != blank and self.kind2 is not None:
            type2 = SubElement(pokemon, 'type')
            type2.text = str(kind_list.index(self.kind2))
        attacks = []
        for i in range(4):
            att = self.attacks[i+1]
            if att is not None:
                attacks.append({})
                attacks[i]['top'] = SubElement(pokemon, 'attacks')
                attacks[i]['id'] = SubElement(attacks[i]['top'], 'id')
                attacks[i]['id'].text = str(i)
                attacks[i]['name'] = SubElement(attacks[i]['top'], 'name')
                attacks[i]['name'].text = str(att.get_name())
                attacks[i]['type'] = SubElement(attacks[i]['top'], 'type')
                attacks[i]['type'].text = str(kind_list.index(att.get_TYP()))
                attacks[i]['power'] = SubElement(attacks[i]['top'], 'power')
                attacks[i]['power'].text = str(att.get_PWR())
                attacks[i]['accuracy'] = SubElement(attacks[i]['top'], 'accuracy')
                attacks[i]['accuracy'].text = str(att.get_ACU())
                attacks[i]['power_points'] = SubElement(attacks[i]['top'], 'power_points')
                attacks[i]['power_points'].text = str(att.get_pp())
        return pokemon

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
            self.kind1 = kind_dict[fo.readline().strip().lower()]
        except KeyError:
            self.kind1 = kind_dict["blank"]
        try:
            self.kind2 = kind_dict[fo.readline().strip().lower()]
        except KeyError:
            self.kind2 = kind_dict["blank"]
        n = self.SPC = int(fo.readline())
        if n > 4:
            n = 4
        for i in range(1, n+1):
            nameh = fo.readline().strip()
            try:
                tp = kind_dict[fo.readline().strip().lower()]
            except KeyError:
                tp = kind_dict["blank"]
            acc = int(fo.readline())
            power = int(fo.readline())
            pp = int(fo.readline())

            self.attacks[i] = Attack(nameh, tp, acc, power, pp, self)
            self.nattack += 1
        fo.close()

    def load(self):
        """ Método para carregar um pokemon da linha de comando. """
        self.name = input().strip()
        self.level = int(input())
        self.HP = int(input())
        self.ATK = int(input())
        self.DEF = int(input())
        self.SPD = int(input())
        self.SPC = int(input())
        try:
            self.kind1 = kind_dict[input().strip().lower()]
        except KeyError:
            self.kind1 = kind_dict["blank"]
        try:
            self.kind2 = kind_dict[input().strip().lower()]
        except KeyError:
            self.kind2 = kind_dict["blank"]
        n = self.SPC = int(input())
        if n > 4:
            n = 4
        for i in range(1, n+1):
            nameh = input().strip()
            try:
                tp = kind_dict[input().strip().lower()]
            except KeyError:
                tp = kind_dict["blank"]
            acc = int(input())
            power = int(input())
            pp = int(input())

            self.attacks[i] = Attack(nameh, tp, acc, power, pp, self)
            self.nattack += 1

    def print_attack(self):
        """ Método que imprime as opções de ataque deste pokemon. """
        if self.left_pp() > 0:
            for i in range(4):
                a = self.attacks[i+1]
                if a is None:
                    break
                params = {"n":i+1, "name":a.get_name(), "pp":a.get_pp(), "ppm":a.get_ppm()}

                if a is not None:
                    print("%(n)d - %(name)s (%(pp)d/%(ppm)d)" % params)
            return True
        return False

    def get_damage(self, damage, kind):
        if self.HP < damage:
            self.HP = 0
        else:
            self.HP -= int(damage)

    def get_nattack(self):
        return self.nattack