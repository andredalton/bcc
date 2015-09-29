__author__ = 'avale'

from pprint import pprint

def main():
    buffer = [4, 2]
    while len(buffer) == 2:
        mapa = {}
        buffer = map(int, raw_input().split())
        if len(buffer) == 2:
            (m, n) = buffer
            for i in range(n):
                (a, b, l) = map(int, raw_input().split())
                if not mapa.has_key(a):
                    mapa[a] = {}
                    mapa[a][b] = l
                elif not mapa[a].has_key(b):
                    mapa[a][b] = l
                elif mapa[a][b] > l:
                    mapa[a][b] = l
                if not mapa.has_key(b):
                    mapa[b] = {}
                    mapa[b][a] = l
                elif not mapa[b].has_key(a):
                    mapa[b][a] = l
                elif mapa[b][a] > l:
                    mapa[b][a] = l
            pprint(mapa)
    pass


if __name__ == '__main__':
    main()