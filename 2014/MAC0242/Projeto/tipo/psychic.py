#! /usr/bin/env python3

from tipo import Tipo

class Psychic(Tipo):
    """ Psychic."""
    def __init__(self):
        self.nome = "Psychic"
        self.efetivo = set(["Fighting", "Poison"])
        self.nao_efetivo = set(["Psychic"])
        self.sem_efeito = set()
        
if __name__ == '__main__':
    pass