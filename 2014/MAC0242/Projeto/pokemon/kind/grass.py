from .kind import Kind

class Grass(Kind):
    """ Grass."""
    def __init__(self):
        self.name = "Grass"
        self.very_effective = set(["Water", "Ground", "Rock"])
        self.not_very_effective = set(["Fire", "Grass", "Poison", "Flying", "Bug", "Dragon"])
        self.not_effective = set()