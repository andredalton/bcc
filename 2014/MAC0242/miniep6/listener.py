from evdev import InputDevice, ecodes, UInput
from select import select

import threading
from keymap import KeyMap

from device import get_devices, flush_input

from pymouse import PyMouse

class Listener:
    def __init__(self, devices):
        # threading.Thread.__init__(self)
        self.devices = devices
        self.listeners = {}
        self.sniffers = {}
        self.msniffers = {}
        self.flag = True
        self.m = PyMouse()

    def add_listener(self, name, tp, code, fun, *args):
        self.listeners[name] = {'type': tp, 'code': code, 'fun': fun, 'args': args}

    def add_key_sniffer(self, name, lst, fun, *args):
        self.sniffers[name] = {'fun': fun, 'args': args, 'lst': lst}

    def del_key_sniffer(self, name):
        self.sniffers.pop(name, None)

    def add_mouse_sniffer(self, name, fun, *args):
        self.msniffers[name] = {'fun': fun, 'args': args}

    def del_mouse_sniffer(self, name):
        self.msniffers.pop(name, None)

    def exit(self):
        self.flag = False

    def run(self):
        while self.flag:
            r,w,x = select(self.devices, [], [])
            for fd in r:
                for event in self.devices[fd].read():
                    lflag = False
                    # for m in self.msniffers.values():
                    #     if event.type == ecodes.EV_SYN and event.code == 00 and event.value == 00:
                    #         print(self.m.position())
                    #         m['fun'](event)
                    # if lflag:
                    #     continue
                    for s in self.sniffers.values():
                        if event.code not in s['lst']:
                            lflag = True
                            # print(event.code)
                            # print(self.m.position())
                            s['fun'](event)
                    if lflag:
                        continue
                    for l in self.listeners.values():
                        if event.type == l["type"] and event.value == 00 and event.code == l["code"]:
                            l['fun'](*l['args'])
                            # lflag = True