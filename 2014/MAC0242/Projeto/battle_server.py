from flask import Flask, abort, request, Response

import os, sys, getopt

from battle import validate, simple_duel, make_battle_state

from pokemon.pokemon import Pokemon

app = Flask(__name__)
pserver = None
pclient = None
client_turn = False
serverxml = ""

@app.route('/')
def index():
    return "Hello, World!"

@app.route('/battle/', methods=['POST'])
def battle():
    global pserver
    global pclient
    global client_turn
    global serverxml
    if pserver is None or pclient is not None:
        abort(423)
    else:
        # Aqui é carregado o XML!
        pclient = Pokemon()
        p = request.form['pokemon']
        pclient.load_xml(validate(p)[0])
        # Define a ordem da batalha
        if pserver.get_SPD() > pclient.get_SPD():
            simple_duel(pserver, pclient)
        else:
            print("Vez do cliente, aguarde!")
        client_turn = True
        
        # Gerando a resposta
        battle_state = make_battle_state(pserver, pclient)
        resp = Response(battle_state, status=200, mimetype='application/xml')
        # Se o pokemon do cliente desmaiou, recarrega o pokemon
        # do servidor e se prepara pra próxima batalha
        if pclient.get_HP()==0:
            if pserver.load_xml(validate(serverxml)[0]) is not None:
                print("Pokemon %s recarregado com sucesso" % pserver.get_name())
        return resp

@app.route('/battle/attack/<int:n>', methods=['GET'])
def attack(n):
    global pserver
    global pclient
    global client_turn
    if pserver is None or pclient is None or not client_turn:
        abort(423)
    else:
        # Realiza o ataque do cliente e do servidor em sequencia.
        simple_duel(pclient, pserver, n)
        simple_duel(pserver, pclient)

        # Retorna o battle_state atual em XML
        battle_state = make_battle_state(pserver, pclient)
        resp = Response(battle_state, status=200, mimetype='application/xml')
        # Recarrega o pokemon do servidor se a batalha tiver terminado.
        if pclient.get_HP()==0 or pserver.get_HP()==0:
            pclient = None
            client_turn = False
            if pserver.load_xml(validate(serverxml)[0]) is not None:
                print("Pokemon %s recarregado com sucesso" % pserver.get_name())
        return resp

if __name__ == '__main__':
    pserver = Pokemon()
    if len(sys.argv)==2:
        serverxml = open(sys.argv[1], "r").read()
        if pserver.load_xml(validate(serverxml)[0]) is not None:
            print("Pokemon %s carregado com sucesso" % pserver.get_name())
            app.run(host='0.0.0.0')