#! /usr/bin/env python3

class Tipo():
    def __init__(self):
        self.nome = ""
        self.efetivo = set()
        self.nao_efetivo = set()
        self.sem_efeito = set()
        
    def get_nome(self):
        return self.nome

    def get_fraqueza(self, tipo):
        if tipo in self.efetivo:
            return 2
        if tipo in self.nao_efetivo:
            return 0.5
        if tipo in self.sem_efeito:
            return 0
        return 1

from tipo.blank import Blank
from tipo.bug import Bug
from tipo.dragon import Dragon
from tipo.eletric import Eletric
from tipo.fighting import Fighting
from tipo.fire import Fire
from tipo.flying import Flying
from tipo.ghost import Ghost
from tipo.grass import Grass
from tipo.ground import Ground
from tipo.ice import Ice
from tipo.normal import Normal
from tipo.poison import Poison
from tipo.psychic import Psychic
from tipo.rock import Rock
from tipo.water import Water

blank = Blank()
bug = Bug()
dragon = Dragon()
eletric = Eletric()
fighting = Fighting()
fire = Fire()
flying = Flying()
ghost = Ghost()
grass = Grass()
ground = Ground()
ice = Ice()
normal = Normal()
poison = Poison()
psychic = Psychic()
rock = Rock()
water = Water()

if __name__ == '__main__':
    pass