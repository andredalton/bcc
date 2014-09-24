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
        diretorios = ["teste"] # Diretório padrão
    for diretorio in diretorios:
        for dirname, dirnames, filenames in os.walk(diretorio):
            for filename in filenames:
                if filename.endswith(".sql"):
                    """ Inicializa as variáveis para um novo teste, carrega o arquivo e gera a ordem adequada. """
                    ordem = []
                    carregadas = set()
                    erro = False
                    with open(diretorio + "/" + filename, "r") as myfile:
                        text=myfile.read().split()
                    dic = miniep3.procedencia(text, False)
                    miniep3.procura(dic, ordem, False)
                    
                    """ Recria o dicionário de procedência para poder comparar com a ordem de inserção."""
                    dic = miniep3.procedencia(text, False)
                    for tabela in ordem:
                        """ Se ocorrer uma inserção cujas referências já não tenham sido carregadas ou seja
                        inexistente considera um erro e para a execução do diagnóstico. Caso contrário insere
                        esta tabela entre as referências já processadas."""
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
