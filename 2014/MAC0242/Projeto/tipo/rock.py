#! /usr/bin/env python3

import tipo

class Rock(tipo.Tipo):
    """ Rock."""
    def __init__(self):
        self.nome = "Rock"
        self.efetivo = set(["Fire", "Ice", "Flying", "Bug"])
        self.nao_efetivo = set(["Fighting", "Ground"])
        self.sem_efeito = set()
        
if __name__ == '__main__':
    pass