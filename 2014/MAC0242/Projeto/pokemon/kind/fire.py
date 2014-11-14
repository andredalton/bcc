from .kind import Kind

class Fire(Kind):
    """ Fire."""
    def __init__(self):
        self.id = 9
        self.name = "Fire"
        self.very_effective = set(["Grass", "Ice", "Bug"])
        self.not_very_effective = set(["Fire", "Water", "Rock", "Dragon"])
        self.not_effective = set()