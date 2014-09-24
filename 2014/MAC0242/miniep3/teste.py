#! /usr/bin/env python3

import miniep3
import sys
import os

def main(argv):
    """ Teste do miniep3.
    
    Procura por todos os arquivos com extensão ".sql" nos diretórios passados como
    argumento."""
    diretorios = argv
    if len(argv) == 0:
        diretorios = ["teste"]
    for diretorio in diretorios:
        for dirname, dirnames, filenames in os.walk(diretorio):
            for filename in filenames:
                if filename.endswith(".sql"):
                    ordem = []
                    carregadas = set()
                    erro = False
                    with open(diretorio + "/" + filename, "r") as myfile:
                        text=myfile.read().split()
                    dic = miniep3.procedencia(text, False)
                    miniep3.procura(dic, ordem, False)
                    if filename == "projetoBD.sql":
                        dic = miniep3.procedencia(text, False)
                    for tabela in ordem:
                        if tabela not in dic:
                            erro = True
                            break
                        if len(dic[tabela]-carregadas) > 0:
                            erro = True
                            break
                        else:
                            carregadas.add(tabela)
                    if erro:
                        print("O teste falhou! =(\t\t", filename)
                    else:
                        print("O teste passou com sucesso!\t", filename)
                    
                
if __name__ == "__main__":
    main(sys.argv[1:])
