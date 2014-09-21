#! /usr/bin/env python3

import sys
import re

def uso():
    """Imprime instruções de uso do programa."""
    uso = """
    Este programa gera a ordem correta de inclusão em um banco de dados.
    Passe o nome dos arquivos na linha de comando.
    """
    print(uso)

def imprime(dic):
    """Imprime um dicionário de listas num formato melhorado."""
    for key in dic.keys():
        print(key + ": ", end="")
        print(', '.join(dic[key]))

def procura(dic, ordem):
    if len(dic) == 0:
        return ordem
    lst = []
    for table in dic.keys():
        if len(dic[table]) == 0:
            lst.append(table)
    if len(lst) == 0:
        print("As tabelas a seguir possuem referência circular:")
        imprime(dic)
        print("\n\nO resultado obtido foi:\n")
        return ordem
    for key in lst:
        ordem.append(key + ".sql")
        del(dic[key])
    for table in dic.keys():
        for key in lst:
            if dic[table].count(key):
                dic[table].remove(key)
    procura(dic, ordem)

def procedencia(lst):
    """Gera uma lista de procedencia para cada tabela.
    
    Inicialmente a função iria trabalhar com o arquivo separado por linhas,
    mas como o arquivo pode ser inteiro feito em apenas uma linha modifiquei
    a estratégia para uma varredura de estados. Não me preocupei com erros de
    sintaxe"""
    
    status = 0
    """Estado do autômato."""
    proc = {}
    """ Dicionário de tabelas procedentes. """
    tabela = ""
    """ Tabela sendo montada no estado atual. """
    fim = re.compile(".*;.*")
    """ Expressão regular que verifica a ocorrência de um delimitador ";".
    Supondo que o delimitador não seja alterado. """
    create = re.compile(".*[cC][rR][eE][aA][tT][eE]$")
    """ Expressão regular que verifica se a palavra atual termina com CREATE. """

    for p in lst:
        if status == 0 and create.match(p):
            status = 1
        elif status == 1:
            if p.lower() == "table":
                status = 2
            else:
                status = 0
        elif status == 2 and p != ";":
            tabela = p.replace("`","")
            proc[tabela] = []
            status = 3
        elif  status == 3 and p.lower() == "foreign":
            status = 4
        elif  status == 4:
            if p.lower() == "key":
                status = 5
            else:
                status = 0
        elif  status == 5 and p.lower() == "references":
            status = 6
        elif status == 6 and p != ";":
            ref = p.replace("`","")
            proc[tabela].append(ref)
            status = 3
        elif fim.match(p):
            if create.match(p):
                status = 1
            else:
                status = 0
                tabela = ""
    return proc

def main(argv):
    """ Função que trata a linha de comando e chama as funcões do programa."""
    ordem = []
    """ Lista que irá conter a ordem de restauração dos arquivos. """
    if len(argv) > 0:
        for arquivo in argv:
            with open(arquivo, "r") as myfile:
                text=myfile.read().split()
            dic = procedencia(text)
            procura(dic, ordem)
            print('\n'.join(ordem))
    else:
        uso()

if __name__ == "__main__":
    main(sys.argv[1:])