__author__ = 'avale'

pais = {}
filhos = {}
suspeitos = []

def mata_filhos(index):
    global filhos
    global suspeitos
    if index in suspeitos:
        suspeitos.remove(index)
    for filho in filhos[index]:
        mata_filhos(filho)

def mata_pais(index):
    global pais
    global suspeitos
    if index in suspeitos:
        suspeitos.remove(index)
    for pai in pais[index]:
        mata_pais(pai)

def killing_in_the_index(index):
    global pais
    global filhos
    global suspeitos
    if index in suspeitos:
        suspeitos.remove(index)
    for pai in pais[index]:
        mata_pais(pai)
    for filho in filhos[index]:
        mata_filhos(filho)

def main():
    global pais
    global filhos
    global suspeitos

    n = int(raw_input())

    for i in range(1, n+1):
        pais[i] = set()
        filhos[i] = set()
        suspeitos.append(i)

    buffer = "fadas"
    while buffer != "BLOOD":
        buffer = raw_input()
        if buffer != "BLOOD":
            (filho, pai) = map(int, buffer.split())
            pais[filho].add(pai)
            filhos[pai].add(filho)

    killingtime = True
    while killingtime:
        try:
            killing_in_the_index(int(raw_input()))
        except EOFError:
            killingtime = False

    if len(suspeitos) > 0:
        for s in suspeitos:
            print s, "",
    else:
        print 0
    return True

if __name__ == "__main__":
    main()