import random
from .attack import Attack
from ..kind import fighting

class Counter(Attack):
    def __init__(self, pp, pokemon):
        self.name = "Counter"
        self.TYP = fighting
        self.PP = pp
        self.PPM = pp
        self.owner = pokemon
        self.target = None
        self.nhit = 0

    def prepare(self, target):
        self.target = target
        self.nhit = self.owner.get_nhits()

    def action(self):
        """ Realiza um Counter. """
        if self.nhit < self.owner.get_nhits():
            param = {"name": self.owner.get_name()}
            print("%(name)s used Counter!" % param)
            self.target.get_damage(2*self.owner.get_last_hit())
        else:
            print("%(name)s don't hit!" % param)
        self.PP -= 1
