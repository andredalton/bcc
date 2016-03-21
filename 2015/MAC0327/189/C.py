import sys

def main():
    all_input = iter(sys.stdin.read().strip().replace('\n', ' ').split(' '))
    n = int(all_input.next())

    a = []
    b = []

    for i in xrange(n):
        e = int(all_input.next())
        a.append(e)

    for i in xrange(n):
        e = int(all_input.next())
        b.append(e)

    pos = 0

    for i in xrange(n):
        # print i, a[i], b[pos:]
        try:
            pos += b[pos:].index(a[i]) + 1
            # print pos
        except ValueError:
            print n-i
            return

    print n
    return


if __name__ == '__main__':
    main()