#! /usr/bin/env python3

import os, sys, getopt

from battle import load_keyboard, printXML
from pokemon.pokemon import Pokemon

from lxml import etree
from lxml.etree import XMLSyntaxError

def main(args):
    p1 = Pokemon()
    
#    parser = etree.XMLParser(dtd_validation=True)

    s = open('battle_state.xsd', "r").read()
    p = open('billpc/rattata', "r").read()


    schema_root = etree.XML(s)
    schema = etree.XMLSchema(schema_root)

    parser = etree.XMLParser(schema = schema)

    try:
        root = etree.fromstring(p, parser)
    except XMLSyntaxError:
        print("Formato XML incorreto!")

    p1.load_xml(root.find("pokemon"))

    print(etree.tostring(root, xml_declaration=True, pretty_print=True))

    # print(p1.kinds[0].get_name())
    # print(p1.kinds[1].get_name())
    # print(len(p1.kinds))

if __name__ == '__main__':
    main(sys.argv[1:])