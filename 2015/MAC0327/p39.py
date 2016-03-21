import sys

def main():
    all_input = iter(sys.stdin.read().strip().replace('\n', ' ').split(' '))
    n = int(all_input.next())

    somas = {}
    max = 0
    for i in xrange(n):
        num = int(all_input.next())
        try:
            somas[num] += 1
        except KeyError:
            somas[num] = 1
        if max < somas[num]:
            max = somas[num]

    # print somas
    # print n - 2 * (n-max), n/2, max


    if n - 2 * (n-max) > 1:
        print "NO"
    else:
        print "YES"

if __name__ == '__main__':
    main()