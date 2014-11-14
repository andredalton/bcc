from .kind import Kind

class Ground(Kind):
    """ Ground."""
    def __init__(self):
        self.id = 4
        self.name = "Ground"
        self.very_effective = set(["Fire", "Eletric", "Poison", "Rock"])
        self.not_very_effective = set(["Grass", "Bug"])
        self.not_effective = set(["Flying"])