#! /usr/bin/env python3

from kind import Kind

class Water(Kind):
    """ Water."""
    def __init__(self):
        self.name = "Water"
        self.very_effective = set(["Fire", "Ground", "Rock"])
        self.not_very_effective = set(["Water", "Grass", "Dragon"])
        self.not_effective = set()
        
if __name__ == '__main__':
    pass