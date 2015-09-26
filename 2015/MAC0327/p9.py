__author__ = 'avale'

filhos = {}
pais = {}

def search_and_destroy():
    global pais
    global filhos
    for i in range(1, len(pais)+1):
        try:
            if len(pais[i]) == 0:
                for f in filhos[i]:
                    pais[f].remove(i)
                pais[i] = None
                return i
        except TypeError:
            pass
    if f + falaram < len(pais):
        search_and_destroy(f + falaram)

def main():
    global pais
    global filhos
    n = int(raw_input())

    for i in range(1, n+1):
        filhos[i] = set()
        pais[i] = set()

    for i in range(1, n+1):
        linha = map(int, raw_input().split())
        linha.remove(0)
        for filho in linha:
            filhos[i].add(filho)

    for i in range(1, n+1):
        for filho in filhos[i]:
            pais[filho].add(i)

    for i in range(n):
        print search_and_destroy(), "",

    return True

if __name__ == "__main__":
    main()
