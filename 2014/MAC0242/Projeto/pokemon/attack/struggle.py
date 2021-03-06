import random
from .attack import Attack
from ..kind import normal

class Struggle(Attack):
    def __init__(self, owner):
        self.name = "Struggle"
        self.TYP = normal
        self.ACU = 100
        self.PWR = 50
        self.PP = 1
        self.owner = owner
        self.target = None

    def decrease_pp(self):
        pass

    def action(self):
        """ Realiza um Struggle. """
        param = {"name": self.owner.get_name(), "attack": self.name}
        print("%(name)s used %(attack)s!" % param)
        d = self.damage()
        self.target.get_damage(d)
        self.owner.get_damage(d/2)
        print("%(name)s's hit with recoil!" % param)
