from evdev import InputDevice, ecodes, UInput

from select import select

import threading

from device import get_devices

class KMLogger(threading.Thread):
    def __init__(self):
        threading.Thread.__init__(self)
        self.devices = get_devices()
        self.listeners = {}

    def add_listener(self, name, tp, code, fun, *args):
        self.listeners[name] = {'type': tp, 'code': code, 'fun': fun, 'args': args}

    def get_listeners(self):
        return self.listeners.keys()

    def del_listener(self, name):
        self.listeners.pop(name, None)

    def run(self):
        flag = True
        while flag:
            r,w,x = select(self.devices, [], [])
            for fd in r:
                print(fd)
                for event in self.devices[fd].read():
                    for l in self.listeners.values():
                        if event.type == l["type"] and event.value == 00 and event.code == l["code"]:
                            print(l['args'])
                            l['fun'](l['args'])
                    if event.type == ecodes.EV_KEY and event.value == 00 and event.code == ecodes.KEY_ESC:
                        print("\rBye!")
                        flag = False

def ppp():
    print("Upa lele")

def soma(a,b):
    print(a+b)

if __name__ == '__main__':
    d = KMLogger()
    d.start()
    d.add_listener("bla", ecodes.EV_KEY, ecodes.KEY_DELETE, print, soma, ppp)
    print(d.get_listeners())
    # d.del_listener("bla")
    # print(d.get_listeners())

    d.join()
    print("ble")
    