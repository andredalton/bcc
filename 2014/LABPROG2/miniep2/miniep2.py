#! /usr/bin/env python3

import sys

def main():
    grammar = "kant.xml"                
    try:                                
        opts, args = getopt.getopt(argv, "hg:d", ["help", "grammar="])
    except getopt.GetoptError:          
        usage()                         
        sys.exit(2)                     
    for opt, arg in opts:
        if opt == '-a':
            print("Buscando a partir dos nomes")
        elif opt == '-u':
            print("Buscando a partir dos UIDs")
        else:
            print("Modo de uso:")

if  __name__ =='__main__':
    main()