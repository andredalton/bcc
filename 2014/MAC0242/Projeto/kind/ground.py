#! /usr/bin/env python3

from kind import Kind

class Ground(Kind):
    """ Ground."""
    def __init__(self):
        self.name = "Ground"
        self.effective = set(["Fire", "Eletric", "Poison", "Rock"])
        self.not_effective = set(["Grass", "Bug"])
        self.uneffective = set(["Flying"])
        
if __name__ == '__main__':
    pass