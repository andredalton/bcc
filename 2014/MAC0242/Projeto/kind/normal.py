#! /usr/bin/env python3

from kind import Kind

class Normal(Kind):
    """ Normal."""
    def __init__(self):
        self.name = "Normal"
        self.effective = set()
        self.not_effective = set(['Rock'])
        self.uneffective = set(["Ghost"])
        
if __name__ == '__main__':
    pass