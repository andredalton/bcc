#! /usr/bin/env python3

import os, sys, getopt

from battle import load_keyboard, printXML
from pokemon.pokemon import Pokemon

def main(args):
    p1 = Pokemon()
    p1.load()
    printXML(p1.make_xml())

if __name__ == '__main__':
    main(sys.argv[1:])