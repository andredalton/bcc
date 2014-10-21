from .kind import Kind

class Fighting(Kind):
    """ Fighting."""
    def __init__(self):
        self.name = "Fighting"
        self.very_effective = set(["Normal", "Ice", "Rock"])
        self.not_very_effective = set(["Poison", "Flying", "Psychic", "Bug"])
        self.not_effective = set(["Ghost"])