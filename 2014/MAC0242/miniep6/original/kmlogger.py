from __future__ import with_statement
from evdev import InputDevice, ecodes, UInput
from pymouse import PyMouse
from select import select
from threading import Thread

import signal
import threading
import pprint
import cPickle as pickle
import time
import contextlib
import sys

# Funcao para dar flush no buffer de entrada
def flush_input():
    try:
        import msvcrt
        while msvcrt.kbhit():
            msvcrt.getch()
    except ImportError:
        import sys, termios
        termios.tcflush(sys.stdin, termios.TCIOFLUSH)

class KMLogger():
    def __init__(self):
        self.devices = self.getDevices()
        self.flag = True
        self.history = []       # Historico dos eventos
        self.m = PyMouse()      # Handlers para eventos de mouse e eventos genericos
        self.copy    = False    # Flag para gravacao
        self.play    = False    # Flag para tocar
        #self.start   = False   # Posicao inicial do cursor do mouse
        self.player  = False    # Thread de controle do player
        try:
		    history = pickle.load(open('event.pkl', 'rb'))
		except IOError:
		    history = []

    def getDevices(self):
        devicesList = []
        for i in range(0, 20):
            try:
                dev = InputDevice('/dev/input/event'+str(i))
            except OSError:
                continue
            keymap = dev.capabilities().get(1)
            if keymap is not None:
                # Verificando se e um mouse com dois botoes
                if ecodes.BTN_MOUSE in keymap and ecodes.BTN_RIGHT in keymap:
                    devicesList.append(dev)
                # Procurando um teclado apropriado
                if ecodes.KEY_DELETE in keymap and ecodes.KEY_PLAYPAUSE in keymap and ecodes.KEY_RIGHTCTRL in keymap and ecodes.KEY_LEFTCTRL in keymap and ecodes.KEY_F12 in keymap and ecodes.KEY_S:
                    devicesList.append(dev)
        return {dev.fd : dev for dev in devicesList}

    def saveLog(self, filename):
        pickle.dump(self.history , open(filename, 'wb'), -1)
        
    def getLog(self, filename):
        self.history = pickle.load(open(filename, 'rb'))

    def run(self):
    	# Ciclo principal do programa.
		while True:
		    r,w,x = select(devices, [], [])
		    for fd in r:
		        for event in devices[fd].read():
		            # F5 - salva os eventos
		            if not copy and not play and event.type == ecodes.EV_KEY and event.value == 00 and event.code == ecodes.KEY_F5:
		                flush_input()
		                filename =  raw_input("\rSalvar arquivo: ")
		                saveLog(filename, history)
		                print "\rSalvo               "
		            # F6 - carrega os eventos.
		            elif not copy and not play and event.type == ecodes.EV_KEY and event.value == 00 and event.code == ecodes.KEY_F6:
		                flush_input()
		                filename =  raw_input("\rCarregar arquivo: ")
		                history = getLog(filename)
		                print "\rCarregado              "
		            # F3 - roda uma vez.
		            elif not copy and not play and event.type == ecodes.EV_KEY and event.value == 00 and event.code == ecodes.KEY_F3:
		                print "\rPlay               "
		                if thr.__class__.__name__ == "playHistoryThread":
		                    thr.Stop()
		                thr = playHistoryThread( 1 )
		                thr.start()
		            # Del - stop.
		            elif event.type == ecodes.EV_KEY and event.value == 00 and event.code == ecodes.KEY_DELETE:
		                print "\rStop               "
		                if thr.__class__.__name__ == "playHistoryThread":
		                    thr.Stop()
		            # Esc - quit.
		            elif event.type == ecodes.EV_KEY and event.value == 00 and event.code == ecodes.KEY_ESC:
		                print "\rBye!"
		                if thr.__class__.__name__ == "playHistoryThread":
		                    thr.Stop()
		                flush_input()
		                sys.exit()
		            
		            # Pause - pause/play.
		            elif not copy and event.type == ecodes.EV_KEY and event.value == 00 and event.code == ecodes.KEY_PAUSE:
		                print "\rPlay/Pause           "
		                if thr.__class__.__name__ == "playHistoryThread":
		                    thr.Sleep()
		            
		            # F12 - Inicia e finaliza gravacao
		            elif event.type == ecodes.EV_KEY and event.code == ecodes.KEY_F12 and event.value == 00:
		                if copy:
		                    print "\rFinalizado                "
		                    saveLog('event.pkl', history)
		                    copy = False
		                else:
		                    if thr.__class__.__name__ == "playHistoryThread":
		                        thr.Stop()
		                    
		                    start = m.position()
		                    if len(history)==0:
		                        print "\rGravando             "
		                    else:
		                        print "\rRegravando        "
		                    history = []
		                    copy = True

		            # grava movimento caso esteja gravando
		            elif event.type == ecodes.EV_SYN and event.code == 00 and event.value == 00 and copy:
		                print(".")
		                history.append( {"mouse": m.position(), "event": event} )
		                
		            # F4 - Play caso nao esteja gravando
		            elif event.type == ecodes.EV_KEY and event.code == ecodes.KEY_F4 and event.value == 00 and not copy and not play:
		                print "\nPlay forever"
		                if thr.__class__.__name__ == "playHistoryThread":
		                    thr.Stop()
		                thr = playHistoryThread( 0 )
		                thr.start()
		                                                
		            # Grava todas as teclas de mouse e teclado
		            elif copy and event.type == ecodes.EV_KEY and event.code != ecodes.KEY_F12:
		                history.append( {"mouse": m.position(), "event": event} )
		flush_input()

class Player(Thread):
    def __init__(self, n, m):
        Thread.__init__(self)
        self.n = n
        self.stop = False
        self.sleep = False
        self.ui = UInput()
        self.m = m
    
    def sendEvent(self):
        i = 0
        while i < len(history):
            now = history[i]
            if i < len(history)-1: after = history[i+1]
                
            if self.stop: break
            if not self.sleep:
                m.move( now.get("mouse")[0], now.get("mouse")[1])
                if now.get("event").type == ecodes.EV_KEY:
                    self.ui.write(ecodes.EV_KEY, now.get("event").code, now.get("event").value)
                    self.ui.syn()
                if i < len(history):
                    time.sleep(float(after.get("event").sec - now.get("event").sec)+float(after.get("event").usec - now.get("event").usec)/1000000)
                i += 1
    
    # sobrescrevendo o metodo run()
    def run(self):
        if self.n == 0:
            while not self.stop:
                self.sendEvent()
        elif self.n > 0:
            for i in range(self.n):
                self.sendEvent()
        self.stop = True
        print "\rEnd               "
        ui.close()
    
    def Stop(self):
        self.stop = True
    
    def Sleep(self):
        self.sleep = not self.sleep