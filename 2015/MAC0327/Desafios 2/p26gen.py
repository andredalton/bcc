__author__ = 'avale'

from random import randint

if __name__ == '__main__':
    crosses = set()
    ouths = set()

    while len(crosses) < 3:
        crosses.add((randint(0, 2), randint(0, 2)))

    while len(ouths) < 3:
        outh = (randint(0, 2), randint(0, 2))
        if outh in crosses:
            continue
        ouths.add(outh)

    mtz = [["#", "#", "#"], ["#", "#", "#"], ["#", "#", "#"]]
    for cross in crosses:
        mtz[cross[0]][cross[1]] = "X"
    for outh in ouths:
        mtz[outh[0]][outh[1]] = "O"

    for l in mtz:
        print "".join(l)