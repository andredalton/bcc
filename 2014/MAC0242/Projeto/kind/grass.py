#! /usr/bin/env python3

from kind import Kind

class Grass(Kind):
    """ Grass."""
    def __init__(self):
        self.name = "Grass"
        self.effective = set(["Water", "Ground", "Rock"])
        self.not_effective = set(["Fire", "Grass", "Poison", "Flying", "Bug", "Dragon"])
        self.uneffective = set()
        
if __name__ == '__main__':
    pass