#! /usr/bin/env python3

from tipo import Tipo

class Dragon(Tipo):
    """ Dragon."""
    def __init__(self):
        self.nome = "Dragon"
        self.efetivo = set(["Dragon"])
        self.nao_efetivo = set()
        self.sem_efeito = set()
        
if __name__ == '__main__':
    pass