from .kind import Kind

class Water(Kind):
    """ Water."""
    def __init__(self):
        self.id = 10
        self.name = "Water"
        self.very_effective = set(["Fire", "Ground", "Rock"])
        self.not_very_effective = set(["Water", "Grass", "Dragon"])
        self.not_effective = set()