#! /usr/bin/env python3

import tipo

class Dragon(tipo.Tipo):
    """ Dragon."""
    def __init__(self):
        self.nome = "Dragon"
        self.efetivo = set(["Dragon"])
        self.nao_efetivo = set()
        self.sem_efeito = set()
        
if __name__ == '__main__':
    pass