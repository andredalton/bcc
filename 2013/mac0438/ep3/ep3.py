#!/usr/bin/python
# -*- coding: UTF8 -*-

import numpy as np

from subprocess import call

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
    def __init__(self, id, N, B, H, T, sem):
        threading.Thread.__init__(self)
        self.id = id
        self.N = N
        self.B = B
        self.H = H
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
        global media_urso
        global fout
        while run<2:
            self.wait()
            self.sem.acquire()
            if run==1:
                dic = {"tmpa": time_machine.getTime()+self.T/2, "id": self.id, "tmpb": time_machine.getTime()+self.T, "paradas": self.N}
                print "T[%(tmpa)d]\tUrso %(id)d:\t\tMetade do pote consumida." %dic
                print "\tAbelhas paradas: %(paradas)d" %dic
                print "\tMedia alimentação urso: " + str(self.mediaAlimentado())
                print "\tMedia abelhas despertam ursos: " + str(abelhas[0].mediaAcorda())

                if fout != 0:
                    outline = str( dic["tmpa"] ) + "\t" + str( self.mediaAlimentado() ) + "\t" + str( abelhas[0].mediaAcorda() ) + "\n"
                    fout.write( outline )
                
                self.alimentado += 1

                print "T[%(tmpb)d]\tUrso %(id)d:\t\tPote consumido." %dic
                print "\tAbelhas paradas: %(paradas)d" %dic
                print "\tMedia alimentação urso: " + str(self.mediaAlimentado())
                print "\tMedia abelhas despertam ursos: " + str(abelhas[0].mediaAcorda())

                if fout != 0:
                    outline = str( dic["tmpb"] ) + "\t" + str( self.mediaAlimentado() ) + "\t" + str( abelhas[0].mediaAcorda() ) + "\n"
                    fout.write( outline )

                time_machine.updateBTime(self.T)
                
                pote = 0
                
                '''
                Aqui as abelhas são acordadas por intermédio da abelha rainha.
                '''
                abelhas[0].signal_all()
                self.roletaUrsa()
            self.sem.release()

    def mediaAlimentado(self):
        return float(sum([ursos[i].alimentado for i in range(self.B)]))/self.B

    def signal(self):
        global sleep
        sleep = False

    def wait(self):
        global run
        global roleta
        global sleep
        global pote
        while( (sleep or roleta!=self.id or pote!=self.H) and run<2 ):
            pass

    def roletaUrsa(self):
        global roleta
        roleta = (roleta + 1) % self.B
    
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
        self.acorda_urso = 0
        self.acesso_pote = 0
        
    def run(self):
        global run
        global time_machine
        global a_pote
        global pote
        global ursos
        global media_abelha
        global fout
        sys.stdout.write( "Nasceu a abelha %(id)d!\n" %{"id": self.id} )
        while run<2:
            self.wait()
            '''
            Semáforo com tamanho fixo de acesso, apenas 100 abelhas ou o tamanho máximo do pote podem estar nesta área.
            '''
            self.sem.acquire()
            if run==1:
                '''
                Semáforo da sessão crítica. Impede que abelhas tenham acesso simultâneo ao pote.
                '''
                self.sem_sc.acquire()
                self.local_time = time_machine.getATime()

                if self.local_time == time_machine.getATime():
                    self.acesso_pote += 1
                    a_pote += 1
                    pote += 1
                    dic = {"tmp": time_machine.getTime(), "id": self.id}
                    status = self.getStatus()
                    if pote == self.H/2:
                        print "T[%(tmp)d]\tAbelha %(id)d:\tPote na metade." %dic
                        print "\tAbelhas voando: %(voando)d" %status
                        print "\tAbelhas trabalhando: %(trabalhando)d" %status
                        print "\tAbelhas paradas: %(paradas)d" %status
                        print "\tMedia alimentação urso: " + str(ursos[0].mediaAlimentado())
                        print "\tMedia abelhas despertam ursos: " + str(self.mediaAcorda())
                    
                    elif pote == self.H and sleep:
                        print "T[%(tmp)d]\tAbelha %(id)d:\tPote cheio." %dic
                        print "\tAbelhas voando: %(voando)d" %status
                        print "\tAbelhas trabalhando: %(trabalhando)d" %status
                        print "\tAbelhas paradas: %(paradas)d" %status
                        print "\tMedia alimentação urso: " + str(ursos[0].mediaAlimentado())
                        print "\tMedia abelhas despertam ursos: " + str(self.mediaAcorda())

                        if fout != 0:
                            outline = str( dic["tmp"] ) + "\t" + str( ursos[0].mediaAlimentado() ) + "\t" + str( self.mediaAcorda() ) + "\n"
                            fout.write( outline )

                        print "\t(As abelhas estão prestes a dormir, o pote será reiniciado.)"
                        if run==1:
                            self.acorda_urso += 1
                            ursos[0].signal()
                            while not sleep and run==1:
                                pass
                            time_machine.updateATime(self.t)
                            a_pote = 0
                            print "\tPote voltou! Liberando acesso às abelhas.\n\n"
                    
                    if a_pote!=0 and ( a_pote==100 or a_pote==self.H or a_pote==self.N):
                        time_machine.updateATime(self.t)
                        a_pote = 0
                self.sem_sc.release()
                
                '''
                Esperam a sincronização aconter.
                '''
                while self.local_time == time_machine.getATime() and run==1:
                    pass

            self.sem.release()

    def getStatus(self):
        global sleep
        global a_pote
        if not sleep:
            return {"paradas":self.N, "trabalhando":0, "voando":0}
        if self.H < 100:
            if self.H > self.N:
                return {"paradas":0, "trabalhando":self.N, "voando":0}
            else:
                return {"paradas":0, "trabalhando":self.H, "voando":self.N-self.H}
        if 100 > self.N:
            return {"paradas":0, "trabalhando":self.N, "voando":0}
        else:
            return {"paradas":0, "trabalhando":100, "voando":self.N-100}

    def mediaAcorda(self):
        return float(sum([abelhas[i].acorda_urso for i in range(self.N)]))/self.N

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
        while (not sleep or a_pote==100 or a_pote==self.H) and run<2:
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

media_urso = []
media_abelha = []

fout = 0

def main():
    global time_machine
    global sleep
    global roleta
    global ursos
    global abelhas
    global run
    global media_abelha
    global media_urso
    global fout

    if len(sys.argv) == 7:
        '''
        Inicializando o modo gráfico.
        '''
        if "-g" == sys.argv[6]:
            '''
            Cria arquivo de saída para os dados gerados. Será usado para gerar os gráficos.
            '''
            fout = open( './ep3.g.data', 'w')

    if len(sys.argv) >= 6:
        [ N, B, H, t, T ] = sys.argv[1:6]
        N = int(N)
        B = int(B)
        H = int(H)
        t = int(t)
        T = int(T)

        time_machine.updateATime(t)

        sem_ursos      = threading.Lock()
        if H < 100:
            sem_abelhas    = threading.BoundedSemaphore(100)
        else:
            sem_abelhas    = threading.BoundedSemaphore(H)
        sem_abelhas_sc = threading.Lock()

        ursos    = [Urso(i, N, B, H, T, sem_ursos) for i in range(B)]
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
        Este programa fornece apenas 10 refeições por urso.
        '''
        while time_machine.getBTime()/T < 10*B:
            pass

        '''
        Envia sinal de término aos monitores.
        '''
        run = 2

        for abelha in abelhas:
            abelha.join()

        for urso in ursos:
            urso.join()

        if fout != 0:
            fout.close()

            fout = open( './ep3.h.data', 'w')
            id = 0
            max = 0
            min = 100000000
            for acessos in [abelhas[i].acesso_pote for i in range(N)]:
                outline = str( id ) + "\t" + str( acessos ) + "\n"
                fout.write( outline ) 
                if acessos > max:
                    max = acessos
                if acessos < min:
                    min = acessos
                id += 1
            fout.close()

            min -= 1
            max += 1

            fout.close()

            fout = open( './ep3.gnuplot.script', 'w')

            fout.write( "#!/usr/bin/gnuplot -persist \n\n" )
            fout.write( "set title \"Média de Alimentação dos " + str(B) + " Ursos pelas " + str(N) + " Abelhas.\\nT=" + str(T) + "     t=" + str(t) + "\"\n" )
            fout.write( "set xlabel \"tempo\"\n" )
            fout.write( "set ylabel \"Alimentados\"\n" )
            fout.write( "set terminal png\n" )
            fout.write( "set key off\n" )
            fout.write( "set output \"./graficos/ursos_" + str(N) + "_" + str(B) + "_" + str(H) + "_" + str(t) + "_" + str(T) + ".png\"\n" )
            fout.write( "plot [0:" + str(time_machine.getTime()) + "] \"./ep3.g.data\" u ($1):($2) w l lt 1 lw 1 \n\n" )
            fout.write( "reset\n" )
            fout.write( "set title \"Média que as " + str(N) + " Abelhas acordaram os " + str(B) + " Ursos.\\nT=" + str(T) + "     t=" + str(t) + "\" \n" )
            fout.write( "set xlabel \"tempo\"\n" )
            fout.write( "set key off\n" )
            fout.write( "set ylabel \"Acordaram/" + str(N) + "\"\n" )
            fout.write( "set terminal png\n" )
            fout.write( "set output \"./graficos/abelhas_" + str(N) + "_" + str(B) + "_" + str(H) + "_" + str(t) + "_" + str(T) + ".png\"\n" )
            fout.write( "plot [0:" + str(time_machine.getTime()) + "] \"./ep3.g.data\" u ($1):($3) w l lt 1 lw 1\n\n" )
            fout.write( "reset\n" )
            fout.write( "set title \"Histograma de acesso das " + str(N) + " Abelhas ao pote.\n" )
            fout.write( "set key off\n" )
            fout.write( "set style fill solid 5.00 border 0\n" )
            fout.write( "set terminal png\n" )
            fout.write( "set xrange [-1:" + str(N) + "]\n" )
            fout.write( "set yrange [" + str(min) +":" + str(max) + "]\n" )
            fout.write( "set output \"./graficos/histograma_" + str(N) + "_" + str(B) + "_" + str(H) + "_" + str(t) + "_" + str(T) + ".png\"\n" )
            fout.write( "bin_width = 1\n" );
            fout.write( "plot './ep3.h.data' u ($1):($2) smooth frequency w boxes\n" );

            fout.close()

            call(["gnuplot", "-persist", "./ep3.gnuplot.script"])

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
        print "\nObs: Este programa oferece apenas 10 refeições por urso."

if __name__ == "__main__":
    main()