class Kind():
    """ Kind. """
    
    def __init__(self):
        """ A classe kind contém um name e suas respectivas efetividades agrupadas. """
        self.id = None
        self.name = ""
        self.very_effective = set()
        self.not_very_effective = set()
        self.not_effective = set()
        
    def get_id(self):
        """ Retorna id do kind. """
        return self.id
        
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

class Eletric(Kind):
    """ Eletric."""
    def __init__(self):
        self.id = 12
        self.name = "Eletric"
        self.very_effective = set(["Water", "Flying"])
        self.not_very_effective = set(["Eletric", "Grass", "Dragon"])
        self.not_effective = set(["Ground"])

class Fighting(Kind):
    """ Fighting."""
    def __init__(self):
        self.id = 1
        self.name = "Fighting"
        self.very_effective = set(["Normal", "Ice", "Rock"])
        self.not_very_effective = set(["Poison", "Flying", "Psychic", "Bug"])
        self.not_effective = set(["Ghost"])

class Blank(Kind):
    """ Blank."""
    def __init__(self):
        self.id = 16
        self.name = ""
        self.very_effective = set()
        self.not_very_effective = set()
        self.not_effective = set()

class Water(Kind):
    """ Water."""
    def __init__(self):
        self.id = 10
        self.name = "Water"
        self.very_effective = set(["Fire", "Ground", "Rock"])
        self.not_very_effective = set(["Water", "Grass", "Dragon"])
        self.not_effective = set()

class Fire(Kind):
    """ Fire."""
    def __init__(self):
        self.id = 9
        self.name = "Fire"
        self.very_effective = set(["Grass", "Ice", "Bug"])
        self.not_very_effective = set(["Fire", "Water", "Rock", "Dragon"])
        self.not_effective = set()

class Rock(Kind):
    """ Rock."""
    def __init__(self):
        self.id = 5
        self.name = "Rock"
        self.very_effective = set(["Fire", "Ice", "Flying", "Bug"])
        self.not_very_effective = set(["Fighting", "Ground"])
        self.not_effective = set()

class Flying(Kind):
    """ Flying."""
    def __init__(self):
        self.id = 2
        self.name = "Flying"
        self.very_effective = set(["Grass", "Fighting", "Bug"])
        self.not_very_effective = set(["Eletric", "Rock"])
        self.not_effective = set()

class Ice(Kind):
    """Ice."""
    def __init__(self):
        self.id = 14
        self.name = "Ice"
        self.very_effective = set(["Grass", "Ground", "Flying", "Dragon"])
        self.not_very_effective = set(["Fire", "Water", "Ice"])
        self.not_effective = set()

class Ground(Kind):
    """ Ground."""
    def __init__(self):
        self.id = 4
        self.name = "Ground"
        self.very_effective = set(["Fire", "Eletric", "Poison", "Rock"])
        self.not_very_effective = set(["Grass", "Bug"])
        self.not_effective = set(["Flying"])

class Ghost(Kind):
    """ Ghost."""
    def __init__(self):
        self.id = 8
        self.name = "Ghost"
        self.very_effective = set(["Psychic", "Ghost"])
        self.not_very_effective = set()
        self.not_effective = set(["Normal"])

class Grass(Kind):
    """ Grass."""
    def __init__(self):
        self.id = 11
        self.name = "Grass"
        self.very_effective = set(["Water", "Ground", "Rock"])
        self.not_very_effective = set(["Fire", "Grass", "Poison", "Flying", "Bug", "Dragon"])
        self.not_effective = set()

class Psychic(Kind):
    """ Psychic."""
    def __init__(self):
        self.id = 13
        self.name = "Psychic"
        self.very_effective = set(["Fighting", "Poison"])
        self.not_very_effective = set(["Psychic"])
        self.not_effective = set()

class Poison(Kind):
    """ Poison."""
    def __init__(self):
        self.id = 3
        self.name = "Poison"
        self.very_effective = set(["Grass"])
        self.not_very_effective = set(["Poison", "Ground", "Rock", "Ghost"])
        self.not_effective = set()

class Normal(Kind):
    """ Normal."""
    def __init__(self):
        self.id = 0
        self.name = "Normal"
        self.very_effective = set()
        self.not_very_effective = set(['Rock'])
        self.not_effective = set(["Ghost"])

class Dragon(Kind):
    """ Dragon."""
    def __init__(self):
        self.id = 15
        self.name = "Dragon"
        self.very_effective = set(["Dragon"])
        self.not_very_effective = set()
        self.not_effective = set()

class Bug(Kind):
    """ Bug."""
    def __init__(self):
        self.id = 7
        self.name = "Bug"
        self.very_effective = set(["Grass", "Psychic"])
        self.not_very_effective = set(["Fire", "Fighting", "Poison", "Flying", "Ghost"])
        self.not_effective = set()

""" Criando uma instância para cada classe herdeira. """
blank = Blank()
bug = Bug()
dragon = Dragon()
eletric = Eletric()
fighting = Fighting()
fire = Fire()
flying = Flying()
ghost = Ghost()
grass = Grass()
ground = Ground()
ice = Ice()
normal = Normal()
poison = Poison()
psychic = Psychic()
rock = Rock()
water = Water()
bird = None

kind_list = [
            normal, fighting, flying, poison,
            ground, rock, bird, bug, ghost,
            fire, water, grass, eletric, psychic,
            ice, dragon, blank
            ]

kind_dict = {
            'blank': blank, 'bug': bug, 'dragon': dragon,
            'eletric': eletric, 'fighting': fighting,
            'fire': fire, 'flying': flying, 'ghost': ghost,
            'grass': grass, 'ground': ground, 'ice': ice,
            'normal': normal, 'poison': poison,
            'psychic': psychic, 'rock': rock, 'water': water, 
            }