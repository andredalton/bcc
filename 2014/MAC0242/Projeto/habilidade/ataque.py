#! /usr/bin/env python3

import random
import os
import sys

#sys.path.append(os.path.abspath(os.path.join(os.path.dirname(__file__), '..')))
#import pokemon
from habilidade import Habilidade

class Ataque(Habilidade):
    """ Ataque. """
    def __init__(self, nome, tipo, acuracia, power, pp):
        self.nome = nome
        self.TYP = tipo
        self.ACU = acuracia
        self.PWR = power
        self.PP = pp

    def critical(self):
        return False
        #return bool(random.getrandbits(1))

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
        alpha = 1
        c = self.critical()
        m = self.modifier(pokemonA, pokemonD)
        if c:
            alpha = (2*pokemonA.get_nivel()+5)/(pokemonA.get_nivel()+5)
            m *= 2
        d = ((2*pokemonA.get_nivel()+10)/250*pokemonA.get_ATK()/pokemonD.get_DEF()*self.PWR+2)*m*alpha
        return d

    def acao(self, pokemonA, pokemonD):
        dano = self.calcula_dano(pokemonA, pokemonD)
        pokemonD.recebe_dano(dano)

if __name__ == '__main__':
    pass