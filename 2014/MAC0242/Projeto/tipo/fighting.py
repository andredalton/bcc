#! /usr/bin/env python3

from tipo import Tipo

class Fighting(Tipo):
    """ Fighting."""
    def __init__(self):
        self.nome = "Fighting"
        self.efetivo = set(["Normal", "Ice", "Rock"])
        self.nao_efetivo = set(["Poison", "Flying", "Psychic", "Bug"])
        self.sem_efeito = set(["Ghost"])
        
if __name__ == '__main__':
    pass