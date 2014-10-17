#! /usr/bin/env python3

from tipo import Tipo

class Eletric(Tipo):
    """ Eletric."""
    def __init__(self):
        self.nome = "Eletric"
        self.efetivo = set(["Water", "Flying"])
        self.nao_efetivo = set(["Eletric", "Grass", "Dragon"])
        self.sem_efeito = set(["Ground"])
        
if __name__ == '__main__':
    pass