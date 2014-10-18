#! /usr/bin/env python3

import os
from subprocess import call

from duel import duel
from pokemon import Pokemon

def clear(int=None):  
    call('clear')
    if int == 0:
       exit()

if __name__ == '__main__':
    print("Bem vindo a maior batalha pokemon de todos os tempos!")
    print("Temos os seguintes pokemons disponíveis:")

    pokemons = []

    for file in os.listdir("billpc/"):
        pokemons.append(Pokemon())
        pokemons[-1].load_file("billpc/"+file)

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

    while pokemons[p1].get_HP() > 0 and pokemons[p2].get_HP() > 0:
        params = {
            "name1":pokemons[p1].get_name(),
            "name2":pokemons[p2].get_name(),
            "hp1":pokemons[p1].get_HP(),
            "hp2":pokemons[p2].get_HP(),
        }
        print("\n%(hp1)d\t- %(name1)s" % params)
        print("%(hp2)s\t- %(name2)s\n" % params)
        
        print("\nAtaques de", pokemons[p1].get_name())
        pokemons[p1].print_attack()
        a1 = None
        while a1 is None:
            try:
                a1 = pokemons[p1].__dict__["action"+str(int(input("Selecione um ataque para " + pokemons[p1].get_name() + ": ")))]
            except ValueError:
                print("Digite um número entre 1 e", pokemons[p1].get_nattack())
        
        print("\nAtaques de", pokemons[p2].get_name())
        pokemons[p2].print_attack()
        a2 = None
        while a2 is None:
            try:
                a2 = pokemons[p2].__dict__["action"+str(int(input("Selecione um ataque para " + pokemons[p2].get_name() + ": ")))]
            except ValueError:
                print("Digite um número entre 1 e", pokemons[p2].get_nattack())
        
        print()
        duel(a1, a2)
        """
        if pokemons[p1].get_SPD() > pokemons[p2].get_SPD():
            a1.action(pokemons[p2])
            if pokemons[p2].get_HP()==0:
                break
            a2.action(pokemons[p1])
        else:
            a2.action(pokemons[p1])
            if pokemons[p1].get_HP()==0:
                break
            a1.action(pokemons[p2])
        """

    print("\nBatalha encerrada!")
    params = {
        "name1":pokemons[p1].get_name(),
        "name2":pokemons[p2].get_name(),
        "vit1": "win" if pokemons[p2].get_HP() == 0 else "fainted",
        "vit2": "win" if pokemons[p1].get_HP() == 0 else "fainted",
    }
    print("\n%(name1)s %(vit1)s" % params)
    print("%(name2)s %(vit2)s\n" % params)