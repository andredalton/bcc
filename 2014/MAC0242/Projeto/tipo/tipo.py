#! /usr/bin/env python3

class Tipo():
    def __init__(self):
        self.nome = ""
        self.efetivo = set()
        self.nao_efetivo = set()
        self.sem_efeito = set()
        
    def get_nome(self):
        return self.nome

    def get_fraqueza(self, tipo):
        if tipo in self.efetivo:
            return 2
        if tipo in self.nao_efetivo:
            return 0.5
        if tipo in self.sem_efeito:
            return 0
        return 1

if __name__ == '__main__':
    pass