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
    def __init__(self, id, T, sem):
        threading.Thread.__init__(self)
        '''
        Monitor.__init__(b_condition)
        '''
        self.id = id
        self.T = T
        self.sem = sem
        self.alimentado = 0
    
    def run(self):
        sys.stdout.write( "Nasceu o urso %(id)d!\n" %{"id": self.id} )
        global b_sleep
        global roleta
        global time_machine
        global run
        while run:
            self.wait()
            self.sem.acquire()
            b_sleep = True
            if run:
                dic = {"tmpa": time_machine.getTime()+self.T/2, "id": self.id, "tmpb": time_machine.getTime()+self.T}
                print "T[%(tmpa)d]\tMetade do pote consumida (Urso %(id)d)." %dic
                print "T[%(tmpb)d]\tPote consumido (Urso %(id)d)." %dic
                self.alimentado += 1
                time_machine.updateBTime(self.T)
                '''
                Aqui deve acordar as abelhas.
                '''
            self.roletaUrsa()
            self.sem.release()

    def signal(self):
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
    
class Abelha( Monitor, threading.Thread ):
    def __init__(self, id, N, H, t, sem):
        '''
        Monitor.__init__(a_condition)
        '''
        threading.Thread.__init__(self)
        self.id = id
        self.N = N
        self.t
        self.sem = sem
        self.acordou = 0
    
    def enchePote():
        pass
    
    def wait(self):
        global a_sleep

        while a_sleep:
            pass

    def signal_all(self):
        pass

    def run(self):
        pass

    def getAlimentou(self):
        return self.alimentou

    def getId(self):
        return self.id

time_machine = 0

pote = 0
a_pote = 0

roleta = 0

b_sleep = True
a_sleep = True

papai_urso = 0
abelha_rainha = 0

run = True


a_condition = threading.Condition()
b_condition = threading.Condition()


def main():
    global time_machine
    global a_sleep
    global b_sleep
    global roleta
    global papai_urso
    global run

    if len(sys.argv) >= 6:
        [ N, B, H, t, T ] = sys.argv[1:]
        print "Numero minimo de parametros de entrada."

        time_machine = DeLorean()
        sem_ursos    = threading.RLock()
        sem_abelhas  = threading.RLock()

        ursos      = [Urso(i, T, sem_ursos) for i in range(B)]
        papai_urso = ursos[0]

        abelhas       = [Abelha(i, N, H, t, sem_ursos) for i in range(N)]
        abelha_rainha = abelhas[0]

        for urso in ursos:
            urso.start()
        
        for abelha in abelhas:
            abelha.start()

        a_sleep = False

        while time_machine.getBTime()/T < 1000:
            pass

        run = False

        for i in xrange(50):
            print "Wakeup, Mother Fuckers!!!"
            ursos[0].signal()

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

    run = False
    print "Fim!!!"

if __name__ == "__main__":
    main()