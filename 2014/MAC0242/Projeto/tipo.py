#! /usr/bin/env python3

import fraqueza

class Tipo():
	""" Tipo."""
    def __init__(self):
    	self.nome = ""
        self.fraqueza = {}

    def set_nome(self, nome):
        self.nome = nome
        self.fraqueza = fraqueza.Fraqueza[self.nome]
        
    def get_nome(self):
        return self.nome

    def get_modifier(self, nome):
        return self.fraqueza[nome]

if __name__ == '__main__':
    pass