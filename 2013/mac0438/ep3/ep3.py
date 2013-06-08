#!/usr/bin/python
# -*- coding: UTF8 -*-

import threading
import time
import sys

def roletaUrsa(B):
    global roleta
    roleta = (roleta + 1) % B

class  DeLorean(threading.Thread):
    """
    Esta classe precissa de 1,21 Gigawatt para funcionar corretamente!
    """
    def __init__(self):
        threading.Thread.__init__(self)
        self.a_time = 0
        self.b_time = 0
    def getTime(self):
        """
        Somente funionará ao alcançar 88 Mph!
        """
        return self.a_time + self.b_time
    def getATime(self):
        return self.a_time
    def getBTime(self):
        return self.b_time
    def setATime(self, tmp):
        self.a_time=tmp
    def updateBTime(self, delta_t):
        self.b_time+=delta_t

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

time_machine = DeLorean();
pote = 0
a_condition = threading.Condition()
b_condition = threading.Condition()

roleta = 0

def main():
    print len(sys.argv)
#    [ N, B, H, t, T ] = sys.argv[1:]
    global time_machine
    print time_machine.getTime()



if __name__ == "__main__":
    main()