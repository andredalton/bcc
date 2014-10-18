#! /usr/bin/env python3

from kind import Kind

class Normal(Kind):
    """ Normal."""
    def __init__(self):
        self.name = "Normal"
        self.very_effective = set()
        self.not_very_effective = set(['Rock'])
        self.not_effective = set(["Ghost"])
        
if __name__ == '__main__':
    pass