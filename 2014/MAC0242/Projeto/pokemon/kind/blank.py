from .kind import Kind

class Blank(Kind):
    """ Blank."""
    def __init__(self):
        self.id = 16
        self.name = ""
        self.very_effective = set()
        self.not_very_effective = set()
        self.not_effective = set()