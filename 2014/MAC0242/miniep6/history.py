from threading import Thread
import pickle
import time

from pymouse import PyMouse

# from evdev import InputDevice, ecodes, UInput
from evdev import UInput, ecodes

class History(Thread):
    def __init__(self):
        Thread.__init__(self)
        self.n = 0
        self.st = True     # Stop
        self.sl = False     # Sleep
        self.ui = UInput()
        self.history = []
        self.m = PyMouse()
        self.flag = True
        self.rec = False
    
    def send_event(self):
        i = 0
        while i < len(self.history):
            now = self.history[i]
            if i < len(self.history)-1: after = self.history[i+1]
            if self.st: break
            if not self.sl:
                self.m.move( now.get("mouse")[0], now.get("mouse")[1])
                if now.get("event").type == ecodes.EV_KEY:
                    self.ui.write(ecodes.EV_KEY, now.get("event").code, now.get("event").value)
                    self.ui.syn()
                if i < len(self.history):
                    time.sleep(float(after.get("event").sec - now.get("event").sec)+float(after.get("event").usec - now.get("event").usec)/1000000)
                i += 1
    
    # sobrescrevendo o metodo run()
    def run(self):
        while self.flag:
            if not self.st:
                if self.n == 0:
                    while not self.stop:
                        self.send_event()
                elif self.n > 0:
                    for i in range(self.n):
                        self.send_event()
                self.st = True
                # print self.history
                print("\nEnd")

    def exit(self):
        self.stop()
        self.flag = False

    def play(self):
        self.st = False

    def stop(self):
        print("Stop")
        self.st = True
    
    def sleep(self):
        if self.sl:
            print("Play")
        else:
            print("Pause")
        self.sl = not self.sleep

    def reset(self):
        self.history = []

    def append_event(self, event):
        # if event.type == ecodes.EV_KEY:
        self.history.append({"mouse": self.m.position(), 'event': event})
        # print(self.m.position())
        # if self.rec:

    def set_n(self, n):
        self.n = n

    def save(self):
        s = raw_input("\nDigite o nome do arquivo: ")
        pickle.dump( self.history, open(s, 'wb'), -1)
        print("Salvo em %s" %s)

    def load(self):
        """ Carrega um historico. """
        s = raw_input("\nDigite o nome do arquivo: ")
        try:
            self.history = pickle.load(open(s, 'rb'))
            print("Carregado de %s" %s)
        except FileNotFoundError:
            print("Arquivo inexistente!")

if __name__ == '__main__':
    h = History()