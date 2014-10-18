#! /usr/bin/env python3

from kind import Kind

class Blank(Kind):
    """ Blank."""
    def __init__(self):
        self.name = ""
        self.very_effective = set()
        self.not_very_effective = set()
        self.not_effective = set()
        
if __name__ == '__main__':
    pass