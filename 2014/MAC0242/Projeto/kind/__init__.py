#! /usr/bin/env python3

class Kind():
    """ Kind. """
    
    def __init__(self):
        """ A classe kind contém um name e suas respectivas efetividades agrupadas. """
        self.name = ""
        self.effective = set()
        self.not_effective = set()
        self.uneffective = set()
        
    def get_name(self):
        """ Retorna name do kind. """
        return self.name

    def get_fraqueza(self, kind):
        """ Retorna a fraqueza deste kind com relação a outro. """
        if kind in self.effective:
            return 2
        if kind in self.not_effective:
            return 0.5
        if kind in self.uneffective:
            return 0
        return 1

""" Importando todas as classes que herdam de kind. """
from kind.blank import Blank
from kind.bug import Bug
from kind.dragon import Dragon
from kind.eletric import Eletric
from kind.fighting import Fighting
from kind.fire import Fire
from kind.flying import Flying
from kind.ghost import Ghost
from kind.grass import Grass
from kind.ground import Ground
from kind.ice import Ice
from kind.normal import Normal
from kind.poison import Poison
from kind.psychic import Psychic
from kind.rock import Rock
from kind.water import Water

""" Criando uma instância para cada classe herdeira. """
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