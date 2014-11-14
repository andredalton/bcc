""" Importando todas as classes que herdam de kind. """
from .blank import Blank
from .bug import Bug
from .dragon import Dragon
from .eletric import Eletric
from .fighting import Fighting
from .fire import Fire
from .flying import Flying
from .ghost import Ghost
from .grass import Grass
from .ground import Ground
from .ice import Ice
from .normal import Normal
from .poison import Poison
from .psychic import Psychic
from .rock import Rock
from .water import Water

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