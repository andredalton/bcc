#! /usr/bin/env python3

import tipo

class Ice(tipo.Tipo):
    """Ice."""
    def __init__(self):
        self.nome = "Ice"
        self.efetivo = set(["Grass", "Ground", "Flying", "Dragon"])
        self.nao_efetivo = set(["Fire", "Water", "Ice"])
        self.sem_efeito = set()
        
if __name__ == '__main__':
    pass