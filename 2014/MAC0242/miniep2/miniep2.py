#! /usr/bin/env python3

import getopt
import sys

def compara(chave, lst):
    """Compara usuários da lista lst através da chave

    A comparacão ocorre montando um dicionário de listas de usuário
    temporáriamente em dic. Uma vez que todo o dicionário está montado
    os índices do dicionário que contém mais do que um elemento são
    copiados para o dicionário de retorno ret."""
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
    """Lê um arquivo e o transforma em uma lista de dicionários.

    O arquivo de entrada deve seguir o formado:
    <usuário>:x:<UID>:<GID>:<Nome do usuário>:<home>:<shell> 
    Desta maneira transformando o arquivo de texto em uma lista
    com o usuário, UID e Nome do usuário de maneira apropriada."""
    lst = []
    for line in sys.stdin:
        usr = str.split(line, ':')[0]
        uid = str.split(line, ':')[2]
        nome = str.split(line, ':')[4]
        lst.append( dict(usuario=usr, uid=uid, nome=nome) ) 
    return lst

def imprime(dic):
    """Imprime o resultado no formato especificado."""
    for key in dic.keys():
        print(key + ": ", end="")
        print(', '.join(dic[key]))

def uso():
    """Imprime instruções de uso do programa."""
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
    """ Função que trata a linha de comando e chama as funcões do programa."""
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
            opt = 'nome'
        elif o in ("-b", "--uid"):
            opt = 'uid'
        else:
            assert False, "Opção inválida"
            sys.exit(2)
        """ Usando as funcẽs desenvolvidas para o EP."""
        lst = leitura()
        dic = compara(opt, lst)
        imprime(dic)

if __name__ == "__main__":
    main(sys.argv[1:])