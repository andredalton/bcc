from .kind import Kind

class Dragon(Kind):
    """ Dragon."""
    def __init__(self):
        self.id = 15
        self.name = "Dragon"
        self.very_effective = set(["Dragon"])
        self.not_very_effective = set()
        self.not_effective = set()