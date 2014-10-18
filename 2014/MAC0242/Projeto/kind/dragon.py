#! /usr/bin/env python3

from kind import Kind

class Dragon(Kind):
    """ Dragon."""
    def __init__(self):
        self.name = "Dragon"
        self.very_effective = set(["Dragon"])
        self.not_very_effective = set()
        self.not_effective = set()
        
if __name__ == '__main__':
    pass