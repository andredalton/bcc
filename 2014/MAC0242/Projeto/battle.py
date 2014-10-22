#! /usr/bin/env python3

import os, sys, getopt

from pokemon.duel import Duel
from pokemon.pokemon import Pokemon

def load_billpc(dir):
    print("Temos os seguintes pokemons disponíveis:")

    pokemons = []

    for file in os.listdir(dir):
        pokemons.append(Pokemon())
        pokemons[-1].load_file(dir+file)

    for i in range(len(pokemons)):
        params = {"n":i+1,"name":pokemons[i].get_name(), "level": pokemons[i].get_level()}
        print("%(n)d - [%(level)d] %(name)s" % params)

    print()
    p1 = -1
    p2 = -1
    while not (p1 >= 0 and p1 < len(pokemons)):
        try:
            p1 = int(input("Digite o número do primeiro pokemon selecionado: ")) -1
        except ValueError:
            print("Digite um número entre 1 e", len(pokemons))
    while not (p2 >= 0 and p2 < len(pokemons)):
        try:
            p2 = int(input("Digite o número do segundo pokemon selecionado: ")) -1
        except ValueError:
            print("Digite um número entre 1 e", len(pokemons))

    return (pokemons[p1], pokemons[p2])

def load_keyboard():
    p1 = Pokemon()
    p2 = Pokemon()
    p1.load()
    p2.load()
    return (p1, p2)

def battle(p1, p2):
    d = Duel()

    while p1.get_HP() > 0 and p2.get_HP() > 0:
        params = {
            "name1":p1.get_name(),
            "name2":p2.get_name(),
            "hp1":p1.get_HP(),
            "hp2":p2.get_HP(),
        }
        print("\n%(hp1)d\t- %(name1)s" % params)
        print("%(hp2)s\t- %(name2)s\n" % params)
        
        if p1.left_pp() > 0:
            print("\nAtaques de", p1.get_name())
            p1.print_attack()
            a1 = None
            while a1 is None:
                try:
                    a1 = p1.select_attack(int(input("Selecione um ataque para " + p1.get_name() + ": ")))
                except ValueError:
                    print("Digite um número entre 1 e", p1.get_nattack())
                except EOFError:
                    a1 = p1.select_attack(int(input()))
                if a1 is None:
                    print("Digite um número entre 1 e", p1.get_nattack())
        else:
            print("%(name)s has no moves left!" % {"name": p1.get_name()})
            a1 = p1.select_attack("struggle")
        
        if p2.left_pp() > 0:
            print("\nAtaques de", p2.get_name())
            p2.print_attack()
            a2 = None
            while a2 is None:
                try:
                    a2 = p2.select_attack(int(input("Selecione um ataque para " + p2.get_name() + ": ")))
                except ValueError:
                    print("Digite um número entre 1 e", p2.get_nattack())
                if a2 is None:
                    print("Digite um número entre 1 e", p2.get_nattack())
        else:
            print("%(name)s has no moves left!" % {"name": p2.get_name()})
            a2 = p2.select_attack("struggle")
        
        print()
        d.duel(a1, a2)

    print("\nBatalha encerrada!")

def uso(name):
    """Imprime instruções de uso do programa."""
    print("""Este programa implementa uma batalha simples entre dois pokemons.

    -h --help       Imprime isto
    -b --billpc     Modifica a localização do diretório que contém os pokemons

Exemplos:

a) Entrada padrão do teclado:
%(name)s

b) Rodando o programa com os pokemons contidos em ./billpc/:
%(name)s -b

c) Selecionando a pasta que contém os pokemons:
%(name)s --billpc=<diretório>
""" % {"name": name})

def main(argv):
    print("Bem vindo a maior batalha pokemon de todos os tempos!")
    p1 = p2 = None
    billpc = "billpc/"
    try:
        opt, args = getopt.getopt(argv, "hb", ["help", "billpc="])
    except (getopt.GetoptError):
        print("Opção inválida")
        uso(sys.argv[0])
        sys.exit(2)

    if len(argv) > 0:
        for o, a in opt:
            if o in ("-h", "--help"):
                uso(sys.argv[0])
                sys.exit()
            elif o == "-b":
                (p1, p2) = load_billpc(billpc)
            elif o == "--billpc":
                billpc = a
                (p1, p2) = load_billpc(billpc)
            else:
                assert False, "Opção inválida"
                sys.exit(2)
    else:
        (p1, p2) = load_keyboard()

    if p1 is None or p2 is None:
        uso(sys.argv[0])
        sys.exit(2)
    battle(p1, p2)    

def teste():
    return ("bla", "ahhh")

if __name__ == '__main__':
    main(sys.argv[1:])
