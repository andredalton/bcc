#! /usr/bin/env python3

from kind import Kind

class Eletric(Kind):
    """ Eletric."""
    def __init__(self):
        self.name = "Eletric"
        self.very_effective = set(["Water", "Flying"])
        self.not_very_effective = set(["Eletric", "Grass", "Dragon"])
        self.not_effective = set(["Ground"])
        
if __name__ == '__main__':
    pass