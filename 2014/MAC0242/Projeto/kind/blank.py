#! /usr/bin/env python3

from kind import Kind

class Blank(Kind):
    """ Blank."""
    def __init__(self):
        self.name = ""
        self.effective = set()
        self.not_effective = set()
        self.uneffective = set()
        
if __name__ == '__main__':
    pass