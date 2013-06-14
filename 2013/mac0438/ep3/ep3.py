#!/usr/bin/python
# -*- coding: UTF8 -*-

import numpy as np

import threading
import matplotlib.pyplot as plt
import matplotlib.animation as animation
import time
import sys

class  DeLorean():
    """
    Esta classe precissa de 1,21 Gigawatt para funcionar corretamente!
    """
    def __init__(self):
        ''' Tempo das abelhas. '''
        self.a_time = 0 
        ''' Tempo dos ursos. '''
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
        threading.Thread.__init__(self)
        self.control = condition
    def wait():
        self.control.wait()
    def signal():
        self.control.notify()
    def signalAll():
        self.control.notifyAll()

class Urso( threading.Thread ):
    def __init__(self, id, T):
        threading.Thread.__init__(self)
        '''
        Monitor.__init__(b_condition)
        '''
        self.id = id
        self.T = T
        self.alimentado = 0
    
    def run(self):
        sys.stdout.write( "Nasceu o urso %(id)d!\n" %{"id": self.id} )
        global b_sleep
        global roleta
        global time_machine
        global run
        while run:
            self.wait()
            if run:
                b_sleep = True
                print "Urso [%(id)d] comendo! %(run)d" %{"id": self.id, "run": run}
                self.alimentado += 1
                time_machine.updateBTime(self.T)

    def signal(self):
        self.roletaUrsa()
        global b_sleep
        b_sleep = False

    def wait(self):
        global run
        global roleta
        global b_sleep
        while( (b_sleep or roleta!=self.id) and run ):
            pass

    def roletaUrsa(self):
        global roleta
        roleta = (roleta + 1) % self.T

    def getAlimentado(self):
        return self.alimentado

    def getId(self):
        return self.id
    
class Abelha( Monitor ):
    def __init__(self, id, N, H, t, a_condition, b_condition):
        Monitor.__init__(a_condition)
        self.id = id
        self.N = N
        self.tatu = N
        self.t
        self.a_condition = a_condition
        self.b_condition = b_condition
        self.alimentou = 0
    
    def enchePote():
        pass
    
    def run(self):
        pass

    def getAlimentou(self):
        return self.alimentou

time_machine = 0
pote = 0
a_pote = 0
a_condition = threading.Condition()
b_condition = threading.Condition()
roleta = 0

b_sleep = True
a_sleep = True

run = True

def main():
    global time_machine
    global b_sleep
    global roleta
    global run
    time_machine = DeLorean()

    ursos = [Urso(i, 10) for i in range(10)]
    
    for urso in ursos:
        urso.start()
    
    for i in xrange(50):
        print roleta
        print "Wakeup, Mother Fuckers!!!"
        ursos[0].signal()
        print time_machine.getTime()

    run = False
    print "Fim!!!"
    sys.exit()

#    [ N, B, H, t, T ] = sys.argv[1:]
    if len(sys.argv) >= 6:
        print "Numero minimo de parametros de entrada."
    if len(sys.argv) == 7:
        if "-g" == sys.argv[6]:
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
        elif "-h" == sys.argv[6]:
            pass
    else:
        print "Modo de usar:"
        print "python "

if __name__ == "__main__":
    main()