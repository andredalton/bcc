from keymap import KeyMap
from listener import Listener

from device import get_devices, flush_input

from evdev import  ecodes
import sys, getopt
import pickle

class KMLoger:
    """ Classe principal """
    def __init__(self):
        self.devices = get_devices()
        self.keymap = KeyMap(self.devices)
        self.listener = Listener(self.devices)
        self.keyfile = ".keymap"    # Arquivo padrão para o mapa de caracteres
        self.listen()

    def listen(self):
        self.listener.add_listener("quit", ecodes.EV_KEY, self.keymap.get_key_by_func("quit"), self.listener.exit)
        self.listener.add_listener("redefine", ecodes.EV_KEY, self.keymap.get_key_by_func("redefine"), self.change_keys)
        
        self.listener.add_listener("save", ecodes.EV_KEY, self.keymap.get_key_by_func("save"), print, "\nsave")
        self.listener.add_listener("load", ecodes.EV_KEY, self.keymap.get_key_by_func("load"), print, "\nload")
        self.listener.add_listener("run", ecodes.EV_KEY, self.keymap.get_key_by_func("run"), print, "\nrun")
        self.listener.add_listener("stop", ecodes.EV_KEY, self.keymap.get_key_by_func("stop"), print, "\nstop")
        self.listener.add_listener("pause", ecodes.EV_KEY, self.keymap.get_key_by_func("pause"), print, "\npause")
        self.listener.add_listener("rec", ecodes.EV_KEY, self.keymap.get_key_by_func("rec"), print, "\nrec")
        self.listener.add_listener("run_forever", ecodes.EV_KEY, self.keymap.get_key_by_func("run_forever"), print, "\nrun_forever")                   

    def start(self):
        self.listener.start()
        self.listener.join()

    def save_key_map(self):
        """ Salva o mapa de caracteres no arquivo padrão. """
        pickle.dump( self.keymap, open(self.keyfile, 'wb'), -1)
        
    def load_key_map(self):
        """ Carrega o mapa de caracteres do arquivo padrão. """
        try:
            self.keymap = pickle.load(open(self.keyfile, 'rb'))
        except FileNotFoundError:
            self.save_key_map()
        self.listen()

    def change_keys(self):
        print("\nAhhhhh\n")
        self.listener.del_all_listener()
        self.keymap.setup()
        self.listen()
        self.save_key_map()
        sys.exit()

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