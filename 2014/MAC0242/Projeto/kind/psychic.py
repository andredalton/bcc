#! /usr/bin/env python3

from kind import Kind

class Psychic(Kind):
    """ Psychic."""
    def __init__(self):
        self.name = "Psychic"
        self.effective = set(["Fighting", "Poison"])
        self.not_effective = set(["Psychic"])
        self.uneffective = set()
        
if __name__ == '__main__':
    pass