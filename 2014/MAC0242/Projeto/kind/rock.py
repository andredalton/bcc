#! /usr/bin/env python3

from kind import Kind

class Rock(Kind):
    """ Rock."""
    def __init__(self):
        self.name = "Rock"
        self.effective = set(["Fire", "Ice", "Flying", "Bug"])
        self.not_effective = set(["Fighting", "Ground"])
        self.uneffective = set()
        
if __name__ == '__main__':
    pass