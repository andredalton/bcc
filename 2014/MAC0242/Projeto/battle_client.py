import sys, re, requests, time

from battle import validate, make_battle_state, simple_duel, command_line
from pokemon.pokemon import Pokemon

def start_connect(battle_state, host, port):
    """ Tenta uma conexão com o servidor. """
    try:
        r = requests.post("http://%(host)s:%(port)s/battle/" % {'host':host, 'port':port}, data=battle_state)
    except requests.exceptions.RequestException:
        print("Servidor não está respondendo! (%(host)s:%(port)s)" % {'host':host, 'port':port})
        sys.exit()
    if r.status_code != 200:
        print("Servidor ocupado, esperando para reconectar!")
        time.sleep(10)
        return start_connect(battle_state, host, port)
    return r

def main(argv):
    """ Controla o fluxo principal de uma batalha com o servidor. """
    (pclient, port, host) = command_line(argv, "localhost")
    if pclient is None:
        print("Pokemon passado incorretamente.\nPrograma encerrado.")
        sys.exit(2)
    pserver = Pokemon()
    print("Pokemon %s carregado com sucesso" % pclient.get_name())
    xml = make_battle_state(pclient)
    xml = re.sub("encoding=['\"].*['\"]", "", xml)
    battle_state = {'pokemon': xml}
    
    # Tenta realizar a primeira conexão assim que o servidor estiver disponível
    r = start_connect(battle_state, host, port)
    pserver.load_xml(validate(r.text)[0])
    pclient.load_xml(validate(r.text)[1])
    
    # Usado para manter um hitórico dos PPs e imprimir esta opcao corretamente caso necessario
    ppmclient = [None]
    # Usado para verificar qual golpe foi utilizado pelo servidor
    ppserver = [None]
    # Usado para identificar se o ataque atingiu o oponente.
    hpclient = pclient.get_HP()
    hpserver = pserver.get_HP()
    # Registrando quanto de PP cada ataque.
    for i in range(1, 5):
        try:
            ppmclient.append(pclient.get_attack(i).get_pp())
        except AttributeError:
            ppmclient.append(None)
        try:
            ppserver.append(pserver.get_attack(i).get_pp())
        except AttributeError:
            ppserver.append(None)

    while pserver.get_HP() > 0 and pclient.get_HP() > 0:
        att = simple_duel(pclient, pserver, None, False, ppmclient)
        r = requests.get("http://%(host)s:%(port)s/battle/attack/%(att)d" % {'host':host, 'port':port, 'att': att})        
        # Recarrega os pokemons com o resultado calculado no servidor
        pserver.load_xml(validate(r.text)[0])
        pclient.load_xml(validate(r.text)[1])
        # Verifica qual golpe foi utilizado pelo servidor e imprime os golpes na ordem correta e se atingiram o inimigo.
        for i in range(1, 5):
            if ppserver[i] is None: break
            if ppserver[i] > pserver.get_attack(i).get_pp():
                if hpserver > pserver.get_HP():
                    print("%(name)s used %(attack)s!" % {"name": pclient.get_name(), "attack": pclient.get_attack(att).get_name()})
                    hpclient = pclient.get_HP()
                else:
                    print("%(name)s used %(attack)s, but it failed." % {"name": pclient.get_name(), "attack": pclient.get_attack(att).get_name()})
                if hpclient > pclient.get_HP():
                    print("%(name)s used %(attack)s!" % {"name": pserver.get_name(), "attack": pserver.get_attack(i).get_name()})
                    hpserver = pserver.get_HP()
                else:
                    print("%(name)s used %(attack)s, but if failed." % {"name": pserver.get_name(), "attack": pserver.get_attack(i).get_name()})
                ppserver[i] = pserver.get_attack(i).get_pp()
    dic = {'server': pserver.get_name(), 'client': pclient.get_name()}
    if pserver.get_HP() == 0:
        print("%(name)s used %(attack)s!" % {"name": pclient.get_name(), "attack": pclient.get_attack(att).get_name()})
        print("%(server)s fainted!\nYou win with %(client)s!" % dic)
    else:
        for i in range(1, 5):
            if ppserver[i] is None: break
            if ppserver[i] > pserver.get_attack(i).get_pp():
                print("%(name)s used %(attack)s!" % {"name": pserver.get_name(), "attack": pserver.get_attack(i).get_name()})
        print("%(client)s fainted!\nServer win with %(server)s!" % dic)

if __name__ == '__main__':
    main(sys.argv[1:])