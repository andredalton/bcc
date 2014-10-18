#! /usr/bin/env python3

from kind import Kind

class Fire(Kind):
    """ Fire."""
    def __init__(self):
        self.name = "Fire"
        self.very_effective = set(["Grass", "Ice", "Bug"])
        self.not_very_effective = set(["Fire", "Water", "Rock", "Dragon"])
        self.not_effective = set()
        
if __name__ == '__main__':
    pass