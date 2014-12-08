#!/usr/bin/env python
# -*- coding: utf-8 -*-

from keymap import KeyMap
from listener import Listener
from history import History

from device import get_devices, flush_input

# from pymouse import PyMouse
from evdev import  ecodes
import sys, getopt
import cPickle as pickle

import threading


class KMLoger(threading.Thread):
    """ Classe principal """
    def __init__(self):
        threading.Thread.__init__(self)
        self.devices = get_devices()
        self.keyfile = ".keymap"    # Arquivo padrão para o mapa de caracteres
        self.keymap = KeyMap(self.devices)
        self.listener = Listener(self.devices)
        self.history = History()
        self.lock = False
        self.listen()
        self.copy = False

    def record(self):
        if self.copy:
            self.copy = False
            print("\rComandos gravados" + " "*80)
            self.listener.del_key_sniffer("rec")
            # self.listener.del_mouse_sniffer("mouse")
        else:
            self.copy = True
            print("\rIniciando gravação" + " "*80)
            self.history.reset()
            self.listener.add_key_sniffer("rec", [self.keymap.get_key_by_func("rec")], self.history.append_event)
            # self.listener.add_mouse_sniffer("mouse", self.history.append_event)

    def pprint(self, *args):
        if not self.copy:
            print(args)

    def listen(self):
        """ Inicializando todas as fun """
        self.listener.add_listener("redefine", ecodes.EV_KEY, self.keymap.get_key_by_func("redefine"), self.change_keys)
        
        self.listener.add_listener("quit", ecodes.EV_KEY, self.keymap.get_key_by_func("quit"), self.exit)
        self.listener.add_listener("copy", ecodes.EV_KEY, self.keymap.get_key_by_func("rec"), self.record)
        self.listener.add_listener("run", ecodes.EV_KEY, self.keymap.get_key_by_func("run"), self.play)
        # self.listener.add_listener("save", ecodes.EV_KEY, self.keymap.get_key_by_func("save"), self.history.save)
        # self.listener.add_listener("load", ecodes.EV_KEY, self.keymap.get_key_by_func("load"), self.history.load)
        # self.listener.add_listener("pause", ecodes.EV_KEY, self.keymap.get_key_by_func("pause"), self.history.sleep)
        # self.listener.add_listener("stop", ecodes.EV_KEY, self.keymap.get_key_by_func("stop"), self.history.stop)
        # self.listener.add_listener("run_forever", ecodes.EV_KEY, self.keymap.get_key_by_func("run_forever"), self.play_forever)
        # self.listener.add_key_sniffer("rec", [self.keymap.get_key_by_func("rec")], self.history.append_event)

        
        # self.listener.add_listener("copy", ecodes.EV_KEY, self.keymap.get_key_by_func("rec"), pprint, "record")
        self.listener.add_listener("save", ecodes.EV_KEY, self.keymap.get_key_by_func("save"), self.pprint, "save")
        self.listener.add_listener("load", ecodes.EV_KEY, self.keymap.get_key_by_func("load"), self.pprint, "load")
        self.listener.add_listener("pause", ecodes.EV_KEY, self.keymap.get_key_by_func("pause"), self.pprint, "sleep")
        self.listener.add_listener("stop", ecodes.EV_KEY, self.keymap.get_key_by_func("stop"), self.pprint, "stop")
        # self.listener.add_listener("run", ecodes.EV_KEY, self.keymap.get_key_by_func("run"), self.pprint, "play")
        self.listener.add_listener("run_forever", ecodes.EV_KEY, self.keymap.get_key_by_func("run_forever"), self.pprint, "play_forever")
        

    def play(self):
    	if not self.copy:
    		print("\rplay" + " "*80)
	        self.history.set_n(1)
	        self.history.play()

    def play_forever(self):
    	if not self.copy:
	        self.history.set_n(0)
	        self.history.play()

    def run(self):
        self.history.start()
        self.listener.run()
        self.history.join()

    def save_key_map(self):
        """ Salva o mapa de caracteres no arquivo padrão. """
        pickle.dump( self.keymap, open(self.keyfile, 'wb'), -1)
    
    def exit(self):
        if not self.copy:
            self.history.exit()
            self.listener.exit()
            flush_input()
            print("\nBye!")

    def load_key_map(self):
        """ Carrega o mapa de caracteres do arquivo padrão. """
        try:
            self.keymap = pickle.load(open(self.keyfile, 'rb'))
        except IOError:
            self.save_key_map()
        self.listen()

    def change_keys(self):
        if not self.copy:
            self.keymap.setup()
            print("\rNovo mapa de teclas salvo.")
            self.save_key_map()
            print("\rPrograma precisa ser reiniciado")
            self.exit()

    def print_keys(self):
        self.keymap.print_keymap()


def usage():
    """Imprime instruções de uso do programa."""
    uso = """
Bem vindo ao kmlogger, o objetivo de uso deste programa é gravar e executar históricos
dos eventos de mouse e teclado do sistema operacional.

    -h --help       Imprime isto.
    -H --history    Carrega um histórico salvo em um arquivo.
    -a --auto       Executa automaticamente o histórico quantas vezes
                    foi passado pelo argumento ou infinatas caso 0.
    """
    print(uso)

def main(argv):
    """ Faz o tratamento da linha de commando. """
    km = KMLoger()
    km.load_key_map()
    km.print_keys()
    # km.change_keys()
    km.start()
    sys.exit()

    if len(argv) > 0:
        try:
            opts, args = getopt.getopt(argv, "hH:a:", ["help", "history=", "auto="])
        except getopt.GetoptError as err:
            print(err)
            usage()
            sys.exit(2)
        for o, a in opts:
            if o in ("-h", "--help"):
                usage()
                sys.exit()
            elif o in ("-H", "--history"):
                print("Load history.")
            elif o in ("-a", "--auto"):
                try:
                    n = int(a)
                    if n < 0:
                        print("Impossível executar o histórico %d vezes!" % n)
                        sys.exit(2)
                    elif n == 0:
                        print("Executando infinitas vezes!")
                    else:
                        print("Autoplay: %d" % n)
                except ValueError:
                    print("O argumento \"%s\" nao e um numero inteiro!" % a)
                    usage()
                    sys.exit(2)
            else:
                assert False, "Opcao nao tratada"
    else:
        print("Voce esta sem argumentos!")

if __name__ == '__main__':
    main(sys.argv[1:])