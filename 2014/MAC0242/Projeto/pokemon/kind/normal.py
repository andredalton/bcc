from .kind import Kind

class Normal(Kind):
    """ Normal."""
    def __init__(self):
        self.id = 0
        self.name = "Normal"
        self.very_effective = set()
        self.not_very_effective = set(['Rock'])
        self.not_effective = set(["Ghost"])