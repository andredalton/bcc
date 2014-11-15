# Usado pra receber arquivos XML

# Usado para validar e manipular XML
from lxml import etree
from lxml.etree import XMLSyntaxError, Element, SubElement

# Os tipos foram ordenados em kind_list e kind_dict é utilizado para retornar o objeto através de um nome.
from .kind import   kind_list, kind_dict, blank, bug, dragon,\
                    eletric, fighting, flying, fire, ghost,\
                    grass, ground, ice, normal, poison, psychic, rock, water

# Agora existe apenas o Attack e uma especialização para o Struggle
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
        self.kinds = []        
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

    def get_kind(self, n):
        return self.kinds[n]

    def get_attack(self, n):
        return self.attacks[n]
    
    def get_damage(self, damage):
        if self.HP < damage:
            self.HP = 0
        else:
            self.HP -= int(damage)

    def get_nattack(self):
        return self.nattack

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

    def load_xml(self, pk):
        """ Recebe um lxml.etree com um pokemon já validado e inicializa o objeto. """
        atb = pk.find("attributes")
        kinds = pk.findall("type")
        attacks = pk.findall("attacks")
        self.name = pk.find("name").text
        self.level = int(pk.find("level").text)
        self.HP = int(atb.find("health").text)
        self.ATK = int(atb.find("attack").text)
        self.DEF = int(atb.find("defense").text)
        self.SPD = int(atb.find("speed").text)
        self.SPC = int(atb.find("special").text)
        for tp in pk.findall("type"):
            try:
                try:
                    self.kinds.append(kind_list[int(tp.text)])
                except KeyError:
                    self.kinds.append(kind_dict["blank"])
            except IndexError:
                pass
        if len(self.kinds)<2:
            self.kinds.append(kind_dict["blank"])
        for att in attacks:
            ida = int(att.find("id").text)
            namea = att.find("name").text
            try:
                tp = kind_list[int(att.find("type").text)]
            except KeyError:
                tp = kind_dict["blank"] 
            power = int(att.find("power").text)
            acc = int(att.find("accuracy").text)
            pp = int(att.find("power_points").text)
            self.attacks[ida] = Attack(namea, tp, acc, power, pp, self)
            self.nattack += 1

    def make_xml(self):
        """ Método que gera um nó XML para este pokemon. """
        pokemon = Element("pokemon")
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
        type1.text = str(kind_list.index(self.kinds[0]))
        if self.kinds[1] != blank and self.kinds[1] is not None:
            type2 = SubElement(pokemon, 'type')
            type2.text = str(kind_list.index(self.kinds[1]))
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