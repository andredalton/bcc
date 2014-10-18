#! /usr/bin/env python3

class Skill():
    """ Acao - Por enquanto apenas attack."""
    def __init__(self, name, owner):
        self.name = name
        self.owner = owner

    def get_name(self):
    	return self.name

    def get_owner(self):
    	return self.owner

    def action(self, pokemonA, pokemonD):
        pass

if __name__ == '__main__':
    pass