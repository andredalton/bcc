from .kind import Kind

class Blank(Kind):
    """ Blank."""
    def __init__(self):
        self.name = ""
        self.very_effective = set()
        self.not_very_effective = set()
        self.not_effective = set()