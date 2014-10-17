#! /usr/bin/env python3

class Habilidade():
    """ Acao - Por enquanto apenas ataque."""
    def __init__(self):
        self.nome = ""

    def get_nome(self):
    	return self.nome

    def acao(self, pokemonA, pokemonD):
        pass

habilidades = {}

def get_habilidade(nome):
    global habilidades
    try:
        return habilidades[nome]
    except KeyError:
        return None

def add_habilidade(hab):
    global habilidades
    habilidades[hab.get_nome()] = hab
    return habilidades[hab.get_nome()]

if __name__ == '__main__':
    pass