from .kind import Kind

class Ice(Kind):
    """Ice."""
    def __init__(self):
        self.id = 14
        self.name = "Ice"
        self.very_effective = set(["Grass", "Ground", "Flying", "Dragon"])
        self.not_very_effective = set(["Fire", "Water", "Ice"])
        self.not_effective = set()