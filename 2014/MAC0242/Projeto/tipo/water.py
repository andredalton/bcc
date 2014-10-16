#! /usr/bin/env python3

import tipo

class Water(tipo.Tipo):
    """ Water."""
    def __init__(self):
        self.nome = "Water"
        self.efetivo = set(["Fire", "Ground", "Rock"])
        self.nao_efetivo = set(["Water", "Grass", "Dragon"])
        self.sem_efeito = set()
        
if __name__ == '__main__':
    pass