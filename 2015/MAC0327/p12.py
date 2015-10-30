__author__ = 'avale'

from pprint import pprint

def main():
    all_input = iter(sys.stdin.read().strip().replace('\n', ' ').split(' '))
    n = int(all_input.next())

    containers = []

    for i in range(n):
        containers.append([])
        for j in range(n):



    pprint(containers)

if __name__ == '__main__':
    main()