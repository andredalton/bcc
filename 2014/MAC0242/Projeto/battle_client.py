#! /usr/bin/env python3

import os, sys, getopt, re, requests

from battle import validate, make_battle_state, simple_duel
from pokemon.pokemon import Pokemon

from lxml import etree
from lxml.etree import XMLSyntaxError

def main(host='localhost', port=5000):
    pclient = Pokemon()
    pserver = Pokemon()
    if len(sys.argv)==2:
        p = open(sys.argv[1], "r").read()
        if pclient.load_xml(validate(p)[0]) is not None:
            print("Pokemon %s carregado com sucesso" % pclient.get_name())
            xml = make_battle_state(pclient)
            xml = re.sub("encoding=['\"].*['\"]", "", xml)
            battle_state = {'pokemon': xml}
            r = requests.post("http://%(host)s:%(port)s/battle/" % {'host':host, 'port':port}, data=battle_state)
            
            pserver.load_xml(validate(r.text)[0])
            pclient.load_xml(validate(r.text)[1])
            
            while pserver.get_HP() > 0 and pclient.get_HP() > 0:
                att = simple_duel(pclient, pserver, None, False)
                r = requests.get("http://%(host)s:%(port)s/battle/attack/%(att)d" % {'host':host, 'port':port, 'att': att})
                
                pserver.load_xml(validate(r.text)[0])
                pclient.load_xml(validate(r.text)[1])
            dic = {'server': pserver.get_name(), 'client': pclient.get_name()}
            if pserver.get_HP() == 0:
                print("%(server)s fainted!\nYou win with %(client)s!" % dic)
            else:
                print("%(client)s fainted!\nServer win with %(server)s!" % dic)
if __name__ == '__main__':
    main()