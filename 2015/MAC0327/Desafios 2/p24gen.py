__author__ = 'avale'

from sys import argv

if __name__ == '__main__':
    if len(argv) == 4:
        n = argv[1]
        max = int(argv[2])
        t = int(argv[3])
        print n
        i = 0
        while i < t:
            for j in range(max, -1, -1):
                print j
                i += 1
                if i == t:
                    break
    print -1