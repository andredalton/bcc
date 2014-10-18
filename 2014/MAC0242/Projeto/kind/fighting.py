#! /usr/bin/env python3

from kind import Kind

class Fighting(Kind):
    """ Fighting."""
    def __init__(self):
        self.name = "Fighting"
        self.effective = set(["Normal", "Ice", "Rock"])
        self.not_effective = set(["Poison", "Flying", "Psychic", "Bug"])
        self.uneffective = set(["Ghost"])
        
if __name__ == '__main__':
    pass