#! /usr/bin/env python3

import tipo

class Ghost(tipo.Tipo):
    """ Ghost."""
    def __init__(self):
        self.nome = "Ghost"
        self.efetivo = set(["Psychic", "Ghost"])
        self.nao_efetivo = set()
        self.sem_efeito = set(["Normal"])
        
if __name__ == '__main__':
    pass