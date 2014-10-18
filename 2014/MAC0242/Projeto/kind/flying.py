#! /usr/bin/env python3

from kind import Kind

class Flying(Kind):
    """ Flying."""
    def __init__(self):
        self.name = "Flying"
        self.effective = set(["Grass", "Fighting", "Bug"])
        self.not_effective = set(["Eletric", "Rock"])
        self.uneffective = set()
        
if __name__ == '__main__':
    pass