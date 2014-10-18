#! /usr/bin/env python3

import random
from skill import Skill

class Attack(Skill):
    """ Attack. """
    def __init__(self, name, typ, acuracia, power, pp):
        self.name = name
        self.TYP = typ
        self.ACU = acuracia
        self.PWR = power
        self.PP = pp
        self.PPM = pp

    def get_pp(self):
        return self.PP

    def get_ppm(self):
        return self.PPM

    def get_TYP(self):
        return self.TYP

    def critical(self, pokemonA):
        """ Retorna o multiplicador de critical hit. """
        if random.random() < pokemonA.get_SPD()/512:
            return (2*pokemonA.get_level()+5)/(pokemonA.get_level()+5)
        return 1

    def special(self):
        """ Retorna se é um attack especial. """
        if self.TYP.get_name() in ["Water", "Fire", "Grass", "Ice", "Eletric", "Psychic", "Dragon"]:
            return True
        return False

    def modifier(self, pokemonA, pokemonD):
        """ Retorna o modificador do cálculo do dano entre dois pokemons. """
        if pokemonA.get_kind1 == self.TYP or pokemonA.get_kind2 == self.TYP:
            m = 1.5
        else:
            m = 1
        m *= self.TYP.get_fraqueza(pokemonD.get_kind1().get_name())
        m *= self.TYP.get_fraqueza(pokemonD.get_kind2().get_name())
        m *= random.uniform(0.85, 1)
        return m

    def damage(self, pokemonA, pokemonD):
        """ Retorna o dano deste attack entre dois pokemons. """
        m = self.modifier(pokemonA, pokemonD)
        c = self.critical(pokemonA)
        if self.special():
            d = ((2*pokemonA.get_level()+10)/250*pokemonA.get_SPAtt()/pokemonD.get_SPDef()*self.PWR+2)*m*c
        else:
            d = ((2*pokemonA.get_level()+10)/250*pokemonA.get_ATK()/pokemonD.get_DEF()*self.PWR+2)*m*c
        return d

    def action(self, pokemonA, pokemonD):
        """ Realiza um attack caso ainda possua PP. """
        if self.PP <= 0:
            return False
        d = self.damage(pokemonA, pokemonD)
        pokemonD.get_damage(d)
        self.PP -= 1
        return True

if __name__ == '__main__':
    pass