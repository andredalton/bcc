#! /usr/bin/env python3

from tipo import Tipo

class Grass(Tipo):
    """ Grass."""
    def __init__(self):
        self.nome = "Grass"
        self.efetivo = set(["Water", "Ground", "Rock"])
        self.nao_efetivo = set(["Fire", "Grass", "Poison", "Flying", "Bug", "Dragon"])
        self.sem_efeito = set()
        
if __name__ == '__main__':
    pass