class Kind():
    """ Kind. """
    
    def __init__(self):
        """ A classe kind contém um name e suas respectivas efetividades agrupadas. """
        self.name = ""
        self.very_effective = set()
        self.not_very_effective = set()
        self.not_effective = set()
        
    def get_name(self):
        """ Retorna name do kind. """
        return self.name

    def get_weakness(self, kind):
        """ Retorna a fraqueza deste kind com relação a outro. """
        if kind in self.very_effective:
            return 2
        if kind in self.not_very_effective:
            return 0.5
        if kind in self.not_effective:
            return 0
        return 1