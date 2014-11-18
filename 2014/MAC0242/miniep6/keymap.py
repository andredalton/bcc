from evdev import InputDevice, ecodes, UInput

from device import get_devices

from select import select

class KeyMap:
    """ Classe que trata as teclas. """
    def __init__(self):
        self.save  = 0
        self.load  = 1
        self.run   = 2
        self.stop  = 3
        self.quit  = 4
        self.pause = 5
        self.rec   = 6
        self.run_forever = 7
        self.redefine = 8
        self.keys = [
                    ecodes.KEY_F5,
                    ecodes.KEY_F6,
                    ecodes.KEY_F3,
                    ecodes.KEY_DELETE,
                    ecodes.KEY_ESC,
                    ecodes.KEY_PAUSE,
                    ecodes.KEY_F12,
                    ecodes.KEY_F4,
                    ecodes.KEY_F1
                    ]
        self.msg = [
                    "Pressione a tecla para salvar",
                    "Pressione a tecla para carregar",
                    "Pressione a tecla para executar",
                    "Pressione a tecla para parar",
                    "Pressione a tecla para sair",
                    "Pressione a tecla para pausar",
                    "Pressione a tecla para gravar",
                    "Pressione a tecla para executar pra sempre",
                    "Pressione a tecla para redefinir"
                    ]
        self.devices = get_devices()

    def setup(self):
        self.keys = []
        i = 0
        while i < len(self.msg):
            print(self.msg[i])
            code = self.get_key_code()
            if code not in self.keys:
                self.keys.append(code)
                i += 1

    def get_key_code(self):
        while True:
            r,w,x = select(self.devices, [], [])
            for fd in r:
                print(fd)
                for event in self.devices[fd].read():
                    if event.type == ecodes.EV_KEY:
                        return event.code 

if __name__ == '__main__':
    print("KeyMap")
    k = KeyMap()
    k.setup()
    print(k.keys)