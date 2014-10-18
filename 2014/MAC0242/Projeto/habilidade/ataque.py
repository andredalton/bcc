#! /usr/bin/env python3

import random

from habilidade import Habilidade

class Ataque(Habilidade):
    """ Ataque. """
    def __init__(self, nome, tipo, acuracia, power, pp):
        self.nome = nome
        self.TYP = tipo
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
        if random.random() < pokemonA.get_SPD()/512:
            return (2*pokemonA.get_nivel()+5)/(pokemonA.get_nivel()+5)
        return 1

    def special(self):
        if self.TYP.get_nome() in ["Water", "Fire", "Grass", "Ice", "Eletric", "Psychic", "Dragon"]:
            return True
        return False

    def modifier(self, pokemonA, pokemonD):
        if pokemonA.get_tipo1 == self.TYP or pokemonA.get_tipo2 == self.TYP:
            m = 1.5
        else:
            m = 1
        m *= self.TYP.get_fraqueza(pokemonD.get_tipo1().get_nome())
        m *= self.TYP.get_fraqueza(pokemonD.get_tipo2().get_nome())
        m *= random.uniform(0.85, 1)
        return m

    def calcula_dano(self, pokemonA, pokemonD):
        m = self.modifier(pokemonA, pokemonD)
        c = self.critical(pokemonA)
        if self.special():
            d = ((2*pokemonA.get_nivel()+10)/250*pokemonA.get_SPAtt()/pokemonD.get_SPDef()*self.PWR+2)*m*c
        else:
            d = ((2*pokemonA.get_nivel()+10)/250*pokemonA.get_ATK()/pokemonD.get_DEF()*self.PWR+2)*m*c
        return d

    def acao(self, pokemonA, pokemonD):
        if self.PP <= 0:
            return False
        dano = self.calcula_dano(pokemonA, pokemonD)
        pokemonD.recebe_dano(dano)
        self.PP -= 1
        return True

if __name__ == '__main__':
    pass