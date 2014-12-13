import random

class Attack:
    """ Attack. """
    def __init__(self, name, typ, acu, power, pp, owner):
        self.name = name
        self.TYP = typ
        self.ACU = acu
        self.PWR = power
        self.PP = pp
        self.PPM = pp
        self.owner = owner
        self.target = None

    def get_name(self):
        return self.name

    def get_owner(self):
        return self.owner

    def get_target(self):
        return self.target

    def prepare(self, target):
        self.target = target
        
    def get_pp(self):
        return self.PP

    def get_ppm(self):
        return self.PPM

    def get_PWR(self):
        return self.PWR

    def get_ACU(self):
        return self.ACU

    def get_TYP(self):
        return self.TYP

    def set_ppm(self, ppm):
        self.PPM = ppm

    def decrease_pp(self):
        if self.pp > 0:
            self.pp -= 1

    def critical(self, pr=False):
        """
        Retorna o multiplicador de critical hit.

        pr: Flag que indica se deve ou nao imprimir as mensagens.
        """
        if random.random() < self.owner.get_SPD()/512:
            if pr: print("Critical hit!")
            return (2*self.owner.get_level()+5)/(self.owner.get_level()+5)
        return 1

    def special(self):
        """ Retorna se é um attack especial. """
        if self.TYP.get_name() in ["Water", "Fire", "Grass", "Ice", "Eletric", "Psychic", "Dragon"]:
            return True
        return False

    def modifier(self, pr=False):
        """
        Retorna o modificador do cálculo do dano entre dois pokemons.

        pr: Flag que indica se deve ou nao imprimir as mensagens.
        """
        m = 1
        m *= self.TYP.get_weakness(self.target.get_kind(0).get_name())
        m *= self.TYP.get_weakness(self.target.get_kind(1).get_name())
        if m > 1:
            if pr: print("It's super effective!")
        if self.owner.get_kind(0) == self.TYP or self.owner.get_kind(1) == self.TYP:
            m *= 1.5
        m *= random.uniform(0.85, 1)
        return m

    def damage(self, pr=False):
        """ Retorna o dano deste ataque entre dois pokemons. """
        m = self.modifier(pr)
        c = self.critical(pr)
        if self.special():
            d = ((2*self.owner.get_level()+10)/250*self.owner.get_SPC()/self.target.get_SPC()*self.PWR+2)*m*c
        else:
            d = ((2*self.owner.get_level()+10)/250*self.owner.get_ATK()/self.target.get_DEF()*self.PWR+2)*m*c
        return d

    def action(self):
        """ Realiza um ataque caso ainda possua PP. """
        if self.PP > 0:
            if self.ACU < random.random()*100:
                print("%(name)s used %(attack)s!" % {"name": self.owner.get_name(), "attack": self.name})
                d = self.damage(True)
                self.target.get_damage(d)
            else:
                print("%(name)s used %(attack)s, but it failed!" % {"name": self.owner.get_name(), "attack": self.name})
            self.PP -= 1
            self.target = None
            return True
        self.target = None
        return False