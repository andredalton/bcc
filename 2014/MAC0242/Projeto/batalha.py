#! /usr/bin/env python3

import os
from pokemon import Pokemon

if __name__ == '__main__':
    print("Bem vindo a maior batalha pokemon de todos os tempos!")
    print("Temos os seguintes pokemons disponíveis:")

    pokemons = []

    for file in os.listdir("billpc/"):
        pokemons.append(Pokemon())
        pokemons[-1].carrega_arquivo("billpc/"+file)

    for i in range(len(pokemons)):
        params = {"n":i+1,"nome":pokemons[i].get_nome(), "nivel": pokemons[i].get_nivel()}
        print("%(n)d - [%(nivel)d] %(nome)s" % params)

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

    if pokemons[p1].get_SPD() > pokemons[p2].get_SPD():
        pa = p2
        pd = p1
    else:
        pa = p1
        pd = p2

    while pokemons[p1].get_HP() > 0 and pokemons[p2].get_HP() > 0:
        pm = pa
        pa = pd
        pd = pa

        params = {
            "nome1":pokemons[p1].get_nome(),
            "nome2":pokemons[p2].get_nome(),
            "hp1":pokemons[p1].get_HP(),
            "hp2":pokemons[p2].get_HP(),
            "turno1": "<<<<" if pa == p1 else "",
            "turno2": "<<<<" if pa == p2 else "",
        }
        print("\n%(hp1)d\t- %(nome1)s %(turno1)s" % params)
        print("%(hp2)d\t- %(nome2)s %(turno2)s\n" % params)
        pokemons[pa].print_ataque()
        a = None
        while a is None:
            try:
                a = pokemons[pa].__dict__["acao"+str(int(input("Digite o número do ataque selecionado: ")))]
            except ValueError:
                print("Digite um número entre 1 e", pokemons[pa].get_nataque())
        a.acao(pokemons[pa], pokemons[pd])

    print("\nBatalha encerrada!")
    params = {
        "nome1":pokemons[p1].get_nome(),
        "nome2":pokemons[p2].get_nome(),
        "vit1": "win" if pa == p1 else "fainted",
        "vit2": "win" if pa == p2 else "fainted",
    }
    print("\n%(nome1)s %(vit1)s" % params)
    print("%(nome2)s %(vit2)s\n" % params)