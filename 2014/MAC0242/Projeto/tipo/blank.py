#! /usr/bin/env python3

import tipo

class Blank(tipo.Tipo):
    """ Blank."""
    def __init__(self):
        self.nome = ""
        self.efetivo = set()
        self.nao_efetivo = set()
        self.sem_efeito = set()
        
if __name__ == '__main__':
    pass