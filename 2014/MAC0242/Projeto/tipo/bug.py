#! /usr/bin/env python3

import tipo

class Bug(tipo.Tipo):
    """ Bug."""
    def __init__(self):
        self.nome = "Bug"
        self.efetivo = set(["Grass", "Psychic"])
        self.nao_efetivo = set(["Fire", "Fighting", "Poison", "Flying", "Ghost"])
        self.sem_efeito = set()
        
if __name__ == '__main__':
    pass