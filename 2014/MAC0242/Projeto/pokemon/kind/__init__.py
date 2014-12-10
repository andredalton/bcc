""" Importando todas as classes que herdam de kind. """
from .kind import Blank
from .kind import Bug
from .kind import Dragon
from .kind import Eletric
from .kind import Fighting
from .kind import Fire
from .kind import Flying
from .kind import Ghost
from .kind import Grass
from .kind import Ground
from .kind import Ice
from .kind import Normal
from .kind import Poison
from .kind import Psychic
from .kind import Rock
from .kind import Water

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