#! /usr/bin/env python3

from tipo import Tipo

class Ice(Tipo):
    """Ice."""
    def __init__(self):
        self.nome = "Ice"
        self.efetivo = set(["Grass", "Ground", "Flying", "Dragon"])
        self.nao_efetivo = set(["Fire", "Water", "Ice"])
        self.sem_efeito = set()
        
if __name__ == '__main__':
    pass