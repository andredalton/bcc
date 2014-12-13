import sys, socket
from flask import Flask, request, Response

# Carregando a classe Pokemon
from pokemon.pokemon import Pokemon
# Tarefas comuns para o servidor e o cliente ficaram no módulo battle.py
from battle import validate, simple_duel, make_battle_state, command_line

# Permitindo apenas erros como mensagens do console.
import logging
log = logging.getLogger('werkzeug')
log.setLevel(logging.ERROR)

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
        resp = Response("", status=423, mimetype='text/plain')
        return resp
    else:
        # Aqui é carregado o XML!
        pclient = Pokemon()
        p = request.form['pokemon']
        pclient.load_xml(validate(p)[0])

        # Define a ordem da batalha
        if pserver.get_SPD() > pclient.get_SPD():
            print("\nServer turn")
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
            pclient = None
            client_turn = False
            if pserver.load_xml(validate(serverxml)[0]) is not None:
                print("Pokemon %s recarregado com sucesso" % pserver.get_name())
        return resp

@app.route('/battle/attack/<int:n>', methods=['GET'])
def attack(n):
    global pserver
    global pclient
    global client_turn
    if pserver is None or pclient is None or not client_turn:
        resp = Response("", status=423, mimetype='text/plain')
        return resp
    else:
        # Realiza o ataque do cliente e do servidor em sequencia.
        print("\nClient turn")
        simple_duel(pclient, pserver, n)
        if pserver.get_HP() > 0:
            print("\nServer turn")
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
            else:
                print("Falha ao recarregar pokemon no servidor!")
        return resp

if __name__ == '__main__':
    (pserver, port, host) = command_line(sys.argv[1:])
    if pserver is not None:
        print("Pokemon %s carregado com sucesso" % pserver.get_name())
        serverxml = make_battle_state(pserver)
        try:
            app.run(host=host, port=port, debug=False)
        except socket.gaierror:
            print("Por favor, passe um hostname valido ou endereco de IP.")
    else:
        print("Pokemon passado incorretamente.\nPrograma encerrado.")