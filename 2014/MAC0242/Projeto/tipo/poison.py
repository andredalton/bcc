#! /usr/bin/env python3

from tipo import Tipo

class Poison(Tipo):
    """ Poison."""
    def __init__(self):
        self.nome = "Poison"
        self.efetivo = set(["Grass"])
        self.nao_efetivo = set(["Poison", "Ground", "Rock", "Ghost"])
        self.sem_efeito = set()
        
if __name__ == '__main__':
    pass