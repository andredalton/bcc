from .kind import Kind

class Bug(Kind):
    """ Bug."""
    def __init__(self):
        self.name = "Bug"
        self.very_effective = set(["Grass", "Psychic"])
        self.not_very_effective = set(["Fire", "Fighting", "Poison", "Flying", "Ghost"])
        self.not_effective = set()