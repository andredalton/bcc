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
    
    def updateATime(self, delta_t):
        self.a_time+=delta_t
    
    def updateBTime(self, delta_t):
        self.b_time+=delta_t

class Urso( threading.Thread ):
    def __init__(self, id, T, sem):
        threading.Thread.__init__(self)
        self.id = id
        self.T = T
        self.sem = sem

        self.alimentado = 0
    
    def run(self):
        sys.stdout.write( "Nasceu o urso %(id)d!\n" %{"id": self.id} )
        global sleep
        global roleta
        global time_machine
        global run
        global abelhas
        global pote
        while run<2:
            self.wait()
            self.sem.acquire()
            if run==1:
                dic = {"tmpa": time_machine.getTime()+self.T/2, "id": self.id, "tmpb": time_machine.getTime()+self.T}
                print "T[%(tmpa)d]\tUrso %(id)d:\t\tMetade do pote consumida." %dic
                print "T[%(tmpb)d]\tUrso %(id)d:\t\tPote consumido." %dic
                self.alimentado += 1
                pote = 0
                time_machine.updateBTime(self.T)
                '''
                Aqui as abelhas são acordadas por intermédio da abelha rainha.
                '''
                abelhas[0].signal_all()
                self.roletaUrsa()
            self.sem.release()

    def signal(self):
        global sleep
        sleep = False

    def wait(self):
        global run
        global roleta
        global sleep
        while( (sleep or roleta!=self.id) and run==1 ):
            pass

    def roletaUrsa(self):
        global roleta
        roleta = (roleta + 1) % self.T
    
class Abelha( threading.Thread ):
    def __init__(self, id, N, H, t, sem, sem_sc):
        '''
        Monitor.__init__(a_condition)
        '''
        threading.Thread.__init__(self)
        self.id = id
        self.N = N
        self.H = H
        self.t = t
        self.sem = sem
        self.sem_sc = sem_sc
        self.local_time = 0
        
    def run(self):
        global run
        global time_machine
        global a_pote
        global pote
        global ursos
        sys.stdout.write( "Nasceu a abelha %(id)d!\n" %{"id": self.id} )
        while run<2:
            self.wait()
            self.sem.acquire()
            if run==1:
                self.sem_sc.acquire()
                self.local_time = time_machine.getATime()

                a_pote += 1
                pote += 1
                
                dic = {"tmp": time_machine.getTime()+self.t, "id": self.id}

                if pote == self.H/2:
                    print "T[%(tmp)d]\tAbelha %(id)d:\tPote na metade." %dic
                elif pote == self.H:
                    print "T[%(tmp)d]\tAbelha %(id)d:\tPote cheio." %dic
                    ursos[0].signal()
                    while not sleep:
                        pass

                if a_pote == 100 or a_pote == self.N:
                    time_machine.updateATime(self.t)
                    a_pote = 0

                self.sem_sc.release()
                
                while self.local_time == time_machine.getATime() and run==1:
                    pass

            self.sem.release()

    def wait(self):
        global sleep
        global a_pote
        global time_machine

        '''
        Aqui segura as abelhas para todas as suas restrições imediantamente antes desta pegar o recurso. As restrições são:

        1) Algum urso acordado;
        2) O pote contém 100 abelhas;
        3) Esta abelha já trabalhou neste tic de tempo.
        '''
        while (not sleep or a_pote == 100) and run==1:
            pass

    def signal_all(self):
        global sleep
        sleep = True

'''
Variavel que guarda o objeto da classe DeLorean que cuida do fluxo de tempo.
'''
time_machine = DeLorean()

'''
Variáveis que controlam a quantidade de mel no pote e a quantidade de abelhas no pote respectivamente.
'''
pote = 0
a_pote = 0

'''
Variável que controla qual dos ursos é o próximo a ser alimentado.
'''
roleta = 0

'''
Variável que significa se todos os ursos estão dormindo.
'''
sleep = True

'''
Variáveis que permitem acesso ao primeiro objeto dos monitores Urso e Abelhas respectivamente.
Permitindo assim que os signal de cada uma das classes possa ser acessado em escopo global.
'''
ursos = []
abelhas = []

'''
Variável que indica o estágio do programa:

0 - Sendo inicializado;
1 - Rodando;
2 - Finalizando.
'''
run = 0

def main():
    global time_machine
    global sleep
    global roleta
    global ursos
    global abelhas
    global run

    if len(sys.argv) >= 6:
        [ N, B, H, t, T ] = sys.argv[1:]
        N = int(N)
        B = int(B)
        H = int(H)
        t = int(t)
        T = int(T)

        time_machine.updateATime(t)

        sem_ursos      = threading.Lock()
        sem_abelhas    = threading.BoundedSemaphore()
        sem_abelhas_sc = threading.Lock()

        ursos    = [Urso(i, T, sem_ursos) for i in range(B)]
        abelhas  = [Abelha(i, N, H, t, sem_abelhas, sem_abelhas_sc) for i in range(N)]

        for urso in ursos:
            urso.start()

        for abelha in abelhas:
            abelha.start()

        '''
        Depois que todos os processos foram inicializados e estão rodando, libera o acesso dos monitores ao recurso.
        '''
        run = 1

        '''
        Espera até que 1000 ursos tenham sido acordados.
        '''
        while time_machine.getBTime()/T < 1000:
            pass

        '''
        Envia sinal de término aos monitores.
        '''
        run = 2

    else:
        print "Modo de usar:"
        print "\tpython " + sys.argv[0] + " N B H t T <-g>"
        print "onde:"
        print "\tN\tnúmero de abelhas (obrigatório);"
        print "\tB\tnúmero de ursos (obrigatório);"
        print "\tH\ttamanho do pote (obrigatório);"
        print "\tt\ttempo gasto por uma abelha no pote (obrigatório);"
        print "\tT\ttempo gasto por um urso pra se alimentar (obrigatório);"
        print "\t-g\tmodo gráfico (opcional)."

    if len(sys.argv) == 7:
        '''
        Inicializando o modo gráfico.
        '''
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

if __name__ == "__main__":
    main()