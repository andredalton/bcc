#! /usr/bin/env python3

import random
from skill import Attack
import kind

class Struggle(Attack):
    def __init__(self, typ, acu, pokemon):
        self.name = "Struggle"
        self.TYP = normal
        self.ACU = 1
        self.PWR = 50
        self.owner = pokemon

    def action(self, pokemonD):
        """ Realiza um Struggle. """
        print("%(name)s used %(attack)s!" % {"name": self.owner.get_name(), "attack": self.name})
        d = self.damage(pokemonD)
        pokemonD.get_damage(d)
        self.owner.get_damage(d/2)
