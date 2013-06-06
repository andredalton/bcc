#!/usr/bin/python

import threading
import time
import sys

pote = 0
a_condition = threading.Condition()
b_condition = threading.Condition()
runtime = 0


class Relogio(threading.Thread):
    def __init__(self):
        threading.Thread.__init__(self)
        self.time = 0
    def getTime(self):
        return self.time
    def updateTime(self, delta_t):
        self.time+=delta_t

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
    def __init__(self, B, H, T, a_condition, b_condition):
        super(Urso, self).__init__(b_condition)
        self.B = B
        self.b = 0
        self.T = T
        self.a_condition = a_condition
    def roletaUrsa():
        self.b = (self.b + 1) % self.B
    def run(self):
        while True:
            self.control.wait()
            roletaUrsa()
    
class Abelha( Monitor ):
    def __init__(self, N, H, t, a_condition, b_condition):
        super(Abelha, self).__init__(a_condition)
        self.N = N
        self.tatu = N
        self.t
        self.b_condition = b_condition
    def enchePote():
        pass
    def run(self):
        pass

def main():
    global runtime
#    [ N, B, H, t, T ] = sys.argv[1:]

    rel = Relogio(2, "2s");
    rel.start()
    rel.updateTime(1)
    print rel.getTime()
    rel.updateTime(1)
    rel.updateTime(1)
    rel.updateTime(1)
    rel.updateTime(1)
    print rel.getTime()
    rel.updateTime(5)
    print rel.getTime()


if __name__ == "__main__":
    main()