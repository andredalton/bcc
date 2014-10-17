#! /usr/bin/env python3

from tipo import Tipo

class Flying(Tipo):
    """ Flying."""
    def __init__(self):
        self.nome = "Flying"
        self.efetivo = set(["Grass", "Fighting", "Bug"])
        self.nao_efetivo = set(["Eletric", "Rock"])
        self.sem_efeito = set()
        
if __name__ == '__main__':
    pass