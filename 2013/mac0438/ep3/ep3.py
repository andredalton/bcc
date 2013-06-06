#!/usr/bin/python

import Queue
import threading
import time
import sys

pote = 0
asleep = False
bsleep = True
runtime = 0

class Monitor(threading.Thread):
    def __init__(self, condition):
        super(Monitor, self).__init__(self)
        self.control = condition
    def wait():
        self.control.wait()
    def signal():
        self.control.notify()
    def signalAll():
        self.control.notifyAll()

class Urso( Monitor ):
    def __init__(self, B, H, T, condition):
        super(Urso, self).__init__(condition)
        self.B = B
        self.b = 0
        self.T = T

    def roletaUrsa():
        self.b = (self.b + 1) % self.B

    def run(self):
        while True:
            self.control.wait()
            roletaUrsa()

        
    
class Abelha( Monitor ):
    def __init__(self, N, H, t, condition):
        super(Abelha, self).__init__(condition)
        self.N = N
        self.tatu = N
        self.t

    def enchePote():
        global pote
        global asleep
        global bsleep
        while asleep:
            pass
        pote += 1
        if pote == H:
            asleep = True
            bsleep = False

    def run(self):
        pass


def main():
    global runtime
    [ N, B, H, t, T ] = sys.argv[1:]

    a_condition = threading.Condition()
    b_condition = threading.Condition()

if __name__ == "__main__":
    main()