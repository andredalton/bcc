#! /usr/bin/env python3

from kind import Kind

class Ghost(Kind):
    """ Ghost."""
    def __init__(self):
        self.name = "Ghost"
        self.very_effective = set(["Psychic", "Ghost"])
        self.not_very_effective = set()
        self.not_effective = set(["Normal"])
        
if __name__ == '__main__':
    pass