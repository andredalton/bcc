#! /usr/bin/env python3

import getopt
import sys
import os

def compara(chave, lst):
    dic = {}
    for line in lst:
        if line[chave] not in dic:
            dic[line[chave]] = [line['usuario']]
        else:
            dic[line[chave]].append(line['usuario'])
    '''
    print(dic.keys)
    sys.exit(2)
    
    for key in dic.keys:
        if len(dic[key]) == 1:
            dic.pop(key, None)
    '''
    return dic

def leitura():
    lst = []
    for line in sys.stdin:
        usr = str.split(line, ':')[0]
        uid = str.split(line, ':')[2]
        nome = str.split(line, ':')[4]
        lst.append( dict(usuario=usr, uid=uid, nome=nome) ) 
    return lst

def uso():
    uso = """
Este programa procura por duplicações nos usuários do sistema.
Para que funcione uma das seguintes opções deve ser passada como argumento em linha de comando.

    -h --help                 Imprime isto
    -a --nome                 Procura por nome diferenciando maiúscula de minúscula
    -b --uid                  Procura por UIDs
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
            print("Nome\n\n")
            lst = leitura()
            dic = compara('nome', lst)
            print(dic)
        elif o in ("-b", "--uid"):
            print("UID\n\n")
            lst = leitura()
        else:
            assert False, "Opção inválida"

if __name__ == "__main__":
    main(sys.argv[1:]) # [1:] slices off the first argument which is the name of the program

