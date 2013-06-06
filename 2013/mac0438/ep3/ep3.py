#!/usr/bin/python

import Gnuplot, Gnuplot.funcutils
import Queue
import threading
import time
import sys

pote = 0
asleep = False
bsleep = True
runtime = 0

def demo():
    """Demonstrate the Gnuplot package."""

    from Numeric import *

    # A straightforward use of gnuplot.  The `debug=1' switch is used
    # in these examples so that the commands that are sent to gnuplot
    # are also output on stderr.

    g = Gnuplot.Gnuplot(debug=1)
    g.title('A simple example') # (optional)
    g('set data style linespoints') # give gnuplot an arbitrary command

    # Plot a list of (x, y) pairs (tuples or a Numeric array would
    # also be OK):

    g.plot([[0,1.1], [1,5.8], [2,3.3], [3,4.2]])
    raw_input('Please press return to continue...\n')

    g.reset()

    # Plot one dataset from an array and one via a gnuplot function;
    # also demonstrate the use of item-specific options:

    x = arange(10, typecode=Float)
    y1 = x**2

    # Notice how this plotitem is created here but used later?  This
    # is convenient if the same dataset has to be plotted multiple
    # times.  It is also more efficient because the data need only be
    # written to a temporary file once.

    d = Gnuplot.Data(x, y1, title='calculated by python', with='points 3 3')
    g.title('Data can be computed by python or gnuplot')
    g.xlabel('x')
    g.ylabel('x squared')

    # Plot a function alongside the Data PlotItem defined above:

    g.plot(Gnuplot.Func('x**2', title='calculated by gnuplot'), d)
    raw_input('Please press return to continue...\n')

    # Save what we just plotted as a color postscript file.

    # With the enhanced postscript option, it is possible to show `x
    # squared' with a superscript (plus much, much more; see `help set
    # term postscript' in the gnuplot docs).  If your gnuplot doesn't
    # support enhanced mode, set `enhanced=0' below.

    g.ylabel('x^2') # take advantage of enhanced postscript mode
    g.hardcopy('gp_test.ps', enhanced=1, color=1)
    print ('\n******** Saved plot to postscript file "gp_test.ps" ********\n')
    raw_input('Please press return to continue...\n')

    g.reset()

    # Demonstrate a 3-d plot:
    # set up x and y values at which the function will be tabulated:

    x = arange(35)/2.0
    y = arange(30)/10.0 - 1.5

    # Make a 2-d array containing a function of x and y.  First create
    # xm and ym which contain the x and y values in a matrix form that
    # can be `broadcast' into a matrix of the appropriate shape:

    xm = x[:,NewAxis]
    ym = y[NewAxis,:]
    m = (sin(xm) + 0.1*xm) - ym**2
    g('set parametric')
    g('set data style lines')
    g('set hidden')
    g('set contour base')
    g.title('An example of a surface plot')
    g.xlabel('x')
    g.ylabel('y')

    # The `binary=1' option would cause communication with gnuplot to
    # be in binary format, which is considerably faster and uses less
    # disk space.  (This only works with the splot command due to
    # limitations of gnuplot.)  `binary=1' is the default, but here we
    # disable binary because older versions of gnuplot don't allow
    # binary data.  Change this to `binary=1' (or omit the binary
    # option) to get the advantage of binary format.

    g.splot(Gnuplot.GridData(m,x,y, binary=0))
    raw_input('Please press return to continue...\n')

    # plot another function, but letting GridFunc tabulate its values
    # automatically.  f could also be a lambda or a global function:

    def f(x,y):
        return 1.0 / (1 + 0.01 * x**2 + 0.5 * y**2)

    g.splot(Gnuplot.funcutils.compute_GridData(x,y, f, binary=0))
    raw_input('Please press return to continue...\n')

    # Explicit delete shouldn't be necessary, but if you are having
    # trouble with temporary files being left behind, try uncommenting
    # the following:
    #del g, d

class Relogio(threading.Thread):
    def __init__(self, delta_t, name):
        threading.Thread.__init__(self)
        #super(Relogio, self).__init__(self)
        self.delta_t = delta_t
        self.name = name

    def run(self):
        for i in range(10):
            time.sleep(1)
            print "\n%(name)s: %(pos)d\n" %{"pos": i, "name": self.name}

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
#    [ N, B, H, t, T ] = sys.argv[1:]

#    a_condition = threading.Condition()
#    b_condition = threading.Condition()

#    r1 = Relogio(2, "2s");
#    r2 = Relogio(2, "3s");

#    r1.start()
#    r2.start()

    demo()

if __name__ == "__main__":
    main()