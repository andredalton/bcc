from evdev import InputDevice, ecodes, UInput

from device import get_devices, flush_input

from select import select

import sys

class KeyMap:
    """ Classe que trata as teclas. """
    def __init__(self, devices):
        self.keys = [
                    {'func': "save", 'code':ecodes.KEY_F5, 'msg': "Pressione a tecla para salvar: ", 'info': "Salvar: %s"},
                    {'func': "load", 'code':ecodes.KEY_F6, 'msg': "Pressione a tecla para carregar: ", 'info': "Carregar: %s"},
                    {'func': "run", 'code':ecodes.KEY_F3, 'msg': "Pressione a tecla para executar: ", 'info': "Executar: %s"},
                    {'func': "stop", 'code':ecodes.KEY_DELETE, 'msg': "Pressione a tecla para parar: ", 'info': "Parar: %s"},
                    {'func': "quit", 'code':ecodes.KEY_ESC, 'msg': "Pressione a tecla para sair: ", 'info': "Sair: %s"},
                    {'func': "pause", 'code':ecodes.KEY_PAUSE, 'msg': "Pressione a tecla para pausar: ", 'info': "Pausar: %s"},
                    {'func': "rec", 'code':ecodes.KEY_F12, 'msg': "Pressione a tecla para gravar: ", 'info': "Gravar: %s"},
                    {'func': "run_forever", 'code':ecodes.KEY_F4, 'msg': "Pressione a tecla para executar pra sempre: ", 'info': "Executar sem parar: %s"},
                    {'func': "redefine", 'code':ecodes.KEY_F2, 'msg': "Pressione a tecla para redefinir: ", 'info': "Redefinir teclas: %s"}
                    ]
        self.devices = devices

    def get_key_by_func(self, func):
        for key in self.keys:
            if key["func"] == func:
                return key["code"]
        return None

    def setup(self):
        kmem = []
        i = 0
        for k in self.keys:
            k['print'] = False
        while i < len(self.keys):
            if not self.keys[i]['print']:
                sys.stdout.write(" "*80)
                sys.stdout.write("\r%s" % self.keys[i]["msg"])
                sys.stdout.flush()
                self.keys[i]['print'] = True
            code = self.get_key_code()
            flush_input()
            if code not in kmem:
                kmem.append(code)
                self.keys[i]["code"] = code
                print("\r%s" % self.keys[i]["msg"] + " %s" % ecodes.KEY[code].replace("KEY_", "") + " "*70)
                i += 1

    def get_key_code(self):
        while True:
            r,w,x = select(self.devices, [], [])
            for fd in r:
                for event in self.devices[fd].read():
                    if event.type == ecodes.EV_KEY:
                        return event.code 

    def print_keymap(self):
        for key in self.keys:
            print(key["info"] % ecodes.KEY[key["code"]].replace("KEY_", ""))

if __name__ == '__main__':
    k = KeyMap()
    k.setup()
    k.print_keymap()