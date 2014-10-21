class Skill():
    """ Acao - Por enquanto apenas attack."""
    def __init__(self, name, owner):
        self.name = name
        self.owner = owner
        self.target = None

    def get_name(self):
        return self.name

    def get_owner(self):
        return self.owner

    def prepare(self, target):
        self.target = target

    def action(self):
        self.target = None