from .kind import Kind

class Psychic(Kind):
    """ Psychic."""
    def __init__(self):
        self.name = "Psychic"
        self.very_effective = set(["Fighting", "Poison"])
        self.not_very_effective = set(["Psychic"])
        self.not_effective = set()