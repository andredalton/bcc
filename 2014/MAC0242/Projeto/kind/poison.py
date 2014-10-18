#! /usr/bin/env python3

from kind import Kind

class Poison(Kind):
    """ Poison."""
    def __init__(self):
        self.name = "Poison"
        self.very_effective = set(["Grass"])
        self.not_very_effective = set(["Poison", "Ground", "Rock", "Ghost"])
        self.not_effective = set()
        
if __name__ == '__main__':
    pass