#! /usr/bin/env python3

from tipo import Tipo

class Fire(Tipo):
    """ Fire."""
    def __init__(self):
        self.nome = "Fire"
        self.efetivo = set(["Grass", "Ice", "Bug"])
        self.nao_efetivo = set(["Fire", "Water", "Rock", "Dragon"])
        self.sem_efeito = set()
        
if __name__ == '__main__':
    pass