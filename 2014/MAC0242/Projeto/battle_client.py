import sys, re, requests, time

from battle import validate, make_battle_state, simple_duel, command_line
from pokemon.pokemon import Pokemon

def start_connect(battle_state, host, port):
    """ Tenta uma conexão com o servidor. """
    try:
        r = requests.post("http://%(host)s:%(port)s/battle/" % {'host':host, 'port':port}, data=battle_state)
    except requests.exceptions.RequestException:
        print("Servidor não está respondendo!")
        sys.exit()
    if r.status_code != 200:
        print("Servidor ocupado, esperando para reconectar!")
        time.sleep(10)
        return start_connect(battle_state, host, port)
    return r

def main(argv, host='localhost', port=5000):
    """ Controla o fluxo principal de uma batalha com o servidor. """
    pclient = command_line(argv)
    if pclient is None:
        print("Pokemon passado incorretamente.\nPrograma encerrado.")
        sys.exit(2)
    pserver = Pokemon()
    print("Pokemon %s carregado com sucesso" % pclient.get_name())
    xml = make_battle_state(pclient)
    xml = re.sub("encoding=['\"].*['\"]", "", xml)
    battle_state = {'pokemon': xml}
    
    r = start_connect(battle_state, host, port)
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
    main(sys.argv[1:])