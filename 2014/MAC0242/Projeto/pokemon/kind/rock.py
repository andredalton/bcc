from .kind import Kind

class Rock(Kind):
    """ Rock."""
    def __init__(self):
        self.name = "Rock"
        self.very_effective = set(["Fire", "Ice", "Flying", "Bug"])
        self.not_very_effective = set(["Fighting", "Ground"])
        self.not_effective = set()