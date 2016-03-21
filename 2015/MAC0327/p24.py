import sys
# from pprint import pprint

def main():
    all_input = iter(sys.stdin.read().strip().replace('\n', ' ').split(' '))
    n = int(all_input.next())
    m = int(all_input.next())

    up = None
    down = 0
    left = 100
    right = 0
    mtz = []
    for i in xrange(n):
        lin = list(all_input.next())
        mtz.append(lin)
        for j in xrange(m):
            if mtz[i][j] == "*":
                if up is None:
                    up = i
                if down < i:
                    down = i
                if left > j:
                    left = j
                if right < j:
                    right = j

    # pprint(mtz)

    for i in xrange(up, down+1):
        # print mtz[i][left:right+1]
        print ''.join(mtz[i][left:right+1])
        # "".join(mtz[i])
        # "".join(mtz[i][left:right+1])
        # for j in xrange(left, right+1):
        #     print mtz[i][j],
        # print ""

if __name__ == '__main__':
    main()