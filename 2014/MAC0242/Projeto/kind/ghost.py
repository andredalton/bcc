#! /usr/bin/env python3

from kind import Kind

class Ghost(Kind):
    """ Ghost."""
    def __init__(self):
        self.name = "Ghost"
        self.effective = set(["Psychic", "Ghost"])
        self.not_effective = set()
        self.uneffective = set(["Normal"])
        
if __name__ == '__main__':
    pass