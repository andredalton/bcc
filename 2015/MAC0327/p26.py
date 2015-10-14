__author__ = 'avale'

import sys
import select

from pprint import pprint



pprint()

if select.select([sys.stdin,],[],[],0.0)[0]:
    print "Have data!"
else:
    print "No data"



def main():
    mtz = [[], [], []]
    i = 0
    while i < 9:
        buff = raw_input()
        for c in buff:
            if c == "X":
                mtz[i/3].append(1)
            elif c == "#":
                mtz[i/3].append(0)
            else:
                mtz[i/3].append(-1)
            i += 1

    lins = []
    for i in xrange(3):
        lins.append(sum(mtz[i]))

    dp = 0
    ds = 0
    cols = [0, 0, 0]
    for i in xrange(3):
        for j in xrange(3):
            if i == j:
                dp += mtz[j][i]
            if i + j == 2:
                ds += mtz[j][i]
            cols[i] += mtz[j][i]

    all = lins + cols + [dp, ds]
    print all


    if 3 in all or 2 in all:
        print "Crosses win"
    elif -3 in all or all.count(-2) > 1:
        print "Ouths win"
    else:
        print "Draw"

if __name__ == '__main__':
    main()