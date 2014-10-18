#! /usr/bin/env python3

from kind import Kind

class Dragon(Kind):
    """ Dragon."""
    def __init__(self):
        self.name = "Dragon"
        self.effective = set(["Dragon"])
        self.not_effective = set()
        self.uneffective = set()
        
if __name__ == '__main__':
    pass