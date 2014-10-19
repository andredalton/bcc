#! /usr/bin/env python3

import random
from skill.attack import Attack

class Struggle(Attack):
    def __init__(self, typ, pokemon):
        self.name = "Struggle"
        self.TYP = typ
        self.ACU = 1
        self.PWR = 50
        self.owner = pokemon

    def action(self, pokemonD):
        """ Realiza um Struggle. """
        param = {"name": self.owner.get_name(), "attack": self.name}
        print("%(name)s used %(attack)s!" % param)
        d = self.damage(pokemonD)
        pokemonD.get_damage(d)
        self.owner.get_damage(d/2)
        print("%(name)s's hit with recoil!" % param)
