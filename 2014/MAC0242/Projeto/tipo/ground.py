#! /usr/bin/env python3

from tipo import Tipo

class Ground(Tipo):
    """ Ground."""
    def __init__(self):
        self.nome = "Ground"
        self.efetivo = set(["Fire", "Eletric", "Poison", "Rock"])
        self.nao_efetivo = set(["Grass", "Bug"])
        self.sem_efeito = set(["Flying"])
        
if __name__ == '__main__':
    pass