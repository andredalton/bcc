#!/usr/bin/python
# -*- coding: UTF8 -*-

import numpy as np

import threading
import matplotlib.pyplot as plt
import matplotlib.animation as animation
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
    
    def run(self):
        for i in range(60):
            print i
            time.sleep(1)

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

time_machine = 0
pote = 0
a_condition = threading.Condition()
b_condition = threading.Condition()

roleta = 0

def main():
    global time_machine
#    [ N, B, H, t, T ] = sys.argv[1:]
    if len(sys.argv) == 7:
        if "-g" == sys.argv[6]:
            time_machine = DeLorean()
            time_machine.start()

            def data_gen():
                t = data_gen.t
                cnt = 0
                while cnt < 1000:
                    yield t, np.sin(2*np.pi*t) * np.exp(-t/10.)
                    cnt+=1
                    t += 0.05
            
            data_gen.t = 0

            fig = plt.figure()
            ax = fig.add_subplot(111)
            line, = ax.plot([], [], lw=2)
            ax.set_ylim(-1.1, 1.1)
            ax.set_xlim(0, 5)
            ax.grid()
            xdata, ydata = [], []
            def run(data):
                # update the data
                t,y = data
                xdata.append(t)
                ydata.append(y)
                xmin, xmax = ax.get_xlim()
                if t >= xmax:
                    ax.set_xlim(xmin, 2*xmax)
                    ax.figure.canvas.draw()
                line.set_data(xdata, ydata)

                return line,

            ani = animation.FuncAnimation(fig, run, data_gen, blit=True, interval=0.00001,
                repeat=False)
            plt.show()



    else:
        time_machine = DeLorean()

    print time_machine


if __name__ == "__main__":
    main()