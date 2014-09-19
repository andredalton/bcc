#! /usr/bin/env python3

import getopt
import sys
import os
import pprint

def compara(chave, lst):
    dic = {}
    ret = {}
    for line in lst:
        if chave == 'nome' and len(str.split(line['nome'])) > 1:
            key = str.split(line['nome'])[0] + " " + str.split(line['nome'])[-1]
        else:
            key = line[chave]
        if key not in dic:
            dic[key] = [line['usuario']]
        else:
            dic[key].append(line['usuario'])
    for key in dic.keys():
        if len(dic[key]) > 1:
            ret[key] = dic[key]

    return ret

def leitura():
    lst = []
    for line in sys.stdin:
        usr = str.split(line, ':')[0]
        uid = str.split(line, ':')[2]
        nome = str.split(line, ':')[4]
        lst.append( dict(usuario=usr, uid=uid, nome=nome) ) 
    return lst

def imprime(dic):
    for key in dic.keys():
        print(key + ": ", end="")
        print(', '.join(dic[key]))

def uso():
    uso = """
Este programa procura por duplicações nos usuários do sistema.
Para que funcione uma das seguintes opções deve ser passada
como argumento em linha de comando.

    -h --help       Imprime isto
    -a --nome       Procura por nome diferenciando maiúscula de minúscula
    -b --uid        Procura por UIDs
    """
    print(uso)
    
def main(argv):
    try:
        opt, args = getopt.getopt(argv, "hab", ["help", "nome", "uid"])
    except (getopt.GetoptError):
        print("Opção inválida")
        uso()
        sys.exit(2)
    for o, a in opt:
        if o in ("-h", "--help"):
            uso()
            sys.exit()
        elif o in ("-a", "--nome"):
            lst = leitura()
            dic = compara('nome', lst)
            pp = pprint.PrettyPrinter(indent=4)
            imprime(dic)
        elif o in ("-b", "--uid"):
            lst = leitura()
            dic = compara('uid', lst)
            imprime(dic)
        else:
            assert False, "Opção inválida"

if __name__ == "__main__":
    main(sys.argv[1:]) # [1:] slices off the first argument which is the name of the program