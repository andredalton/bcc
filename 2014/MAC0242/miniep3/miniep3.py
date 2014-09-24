#! /usr/bin/env python3

import sys
import re

def uso():
    """Imprime instruções de uso do programa."""
    uso = """
    Este programa gera a ordem correta de inclusão em um banco de dados.
    Passe o nome dos arquivos na linha de comando. Caso queira imprimir
    ocorrências de referência circular ou inexistente utilize a opção -v.
    """
    print(uso)

def imprime(dic):
    """Imprime um dicionário de listas num formato melhorado."""
    for key in dic.keys():
        print(key + ": ", end="")
        print(', '.join(dic[key]))

def procura(dic, ordem, verb):
    """Procura uma ordem correta de inclusão enquanto existirem tabelas cujas todas as referências já
    tenham sido processadas. Quando em modo detalhado imprime a ocorrência de referência circular ou
    inexistente."""
    if len(dic) == 0:
        """ Busca finalizada. """
        return ordem
    lst = set()
    """ Tabelas a serem removidas nesta iteração. """
    for table in dic.keys():
        if len(dic[table]) == 0:
            lst.add(table)
    if len(lst) == 0:
        """ Caso todas as tabelas restantes possuam referências a serem processadas restaram apenas
        referências inexistentes ou circulares."""
        if verb:
            print("\nAs tabelas a seguir possuem referência circular ou inexistente:")
            imprime(dic)
            print("\nO resultado obtido foi:")
        return ordem
    for key in lst:
        ordem.append(key)
        del(dic[key])
    for table in dic.keys():
        for key in lst:
            if key in dic[table]:
                dic[table].remove(key)
    procura(dic, ordem, verb)

def procedencia(lst, verb):
    """Gera uma lista de procedencia para cada tabela.
    
    Inicialmente a função iria trabalhar com o arquivo separado por linhas,
    mas como o arquivo pode ser inteiro feito em apenas uma linha modifiquei
    a estratégia para uma varredura de estados. Não me preocupei com erros de
    sintaxe.
    
    Lista de estados:
    
    0: Procurando por uma instrução CREATE
    1: Verificando se é uma instrução de criação de tabela TABLE
    2: Procurando o nome da tabela que está sendo criada, contando que diferente de ";"
    3: Procurando se é uma referência a criação de chave estrangeira FOREIGN
    4: Verificando se é uma referência a criação de chave estrangeira KEY
    5: Procurando as referências REFERENCES
    6: Procurando o nome da tabela de referência, contando que diferente de ";"
    7: Próxima palavra é o novo delimitador
    final: Caso ocorra uma instrução com o delimitador encerra a criação da tabela
    """
    delimitador = ";"
    status = 0
    """Estado inicial do autômato."""
    proc = {}
    """ Dicionário de procedentes. """
    tabela = ""
    """ Tabela sendo montada no estado atual. """
    fim = re.compile(".*" + delimitador + ".*")
    """ Expressão regular que verifica a ocorrência de um delimitador ";".
    Supondo que o delimitador não seja alterado. """
    create = re.compile(".*[cC][rR][eE][aA][tT][eE]$")
    """ Expressão regular que verifica se a palavra atual termina com CREATE. """
    delim = re.compile(".*[dD][eE][lL][iI][mM][iI][tT][eE][rR]$")
    """ Expressão regular que verifica se a palavra atual termina com DELIMITER. """

    for p in lst:
        if status == 0 and create.match(p):
            status = 1
        if status == 0 and delim.match(p):
            status = 7
        elif status == 1:
            if p.lower() == "table":
                status = 2
            else:
                status = 0
        elif status == 2 and p != ";":
            tabela = p.replace("`","")
            if tabela in proc and verb:
                print("TABELA " + tabela + " RECRIADA")
            proc[tabela] = set()
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
            proc[tabela].add(ref)
            status = 3
        elif status == 7:
            delimitador = p
            fim = re.compile(".*" + delimitador + ".*")
            status = 0
        elif fim.match(p):
            if create.match(p):
                status = 1
            else:
                status = 0
                tabela = ""
    return proc

def main(argv):
    veb = False
    if "-v" in argv:
        veb = True
    """ Função que trata a linha de comando e chama as funcões do programa."""
    ordem = []
    """ Lista que irá conter a ordem de restauração dos arquivos. """
    if len(argv) > 0:
        for arquivo in argv:
            if arquivo == "-v":
                continue
            ordem = []
            if len(argv) > 1:
                print("\nARQUIVO: " + arquivo)
            with open(arquivo, "r") as myfile:
                text=myfile.read().split()
            dic = procedencia(text, veb)
            procura(dic, ordem, veb)
            print('.sql\n'.join(ordem), end=".sql\n")
    else:
        uso()

if __name__ == "__main__":
    main(sys.argv[1:])