__author__ = 'avale'


def padrao2int(padrao):
    h = 0
    for i in range(16):
        if padrao[i % 4][i / 4]:
            h += 2 ** i
    return h


def main():
    visitados = [False] * 2 ** 16
    padrao = []
    movimento = set()
    m1 = []

    for i in range(4):
        padrao.append(map(lambda x: x == "W", list(raw_input())))

    for i in range(3):
        m1.append(map(lambda x: x == "1", list(raw_input())))

    m1[0].append(False)
    m1[1].append(False)
    m1[2].append(False)
    m1[0].append(False)
    m1[1].append(False)
    m1[2].append(False)
    m1[0].insert(0, False)
    m1[1].insert(0, False)
    m1[2].insert(0, False)
    m1[0].insert(0, False)
    m1[1].insert(0, False)
    m1[2].insert(0, False)
    m1.append([False] * 7)
    m1.append([False] * 7)
    m1.insert(0, [False] * 7)
    m1.insert(0, [False] * 7)

    for i in range(4):
        for j in range(4):
            movimento.add(padrao2int([x[i:i+4] for x in m1[j:j+4]]))

    padrao = [padrao2int(padrao)]
    if padrao[0] == 0 or padrao[0] == 2 ** 16 - 1:
        print 0
    else:
        visitados[padrao[0]] = True
        movimentos = 1
        while (len(padrao) > 0):
            npadrao = []
            for p in padrao:
                for m in movimento:
                    a = p ^ m
                    if a == 0 or a == 2 ** 16 - 1:
                        print movimentos
                        return True
                    if not visitados[a]:
                        visitados[a] = True
                        npadrao.append(a)
            padrao = npadrao
            movimentos += 1
        print "Impossible"
    return False


if __name__ == '__main__':
    main()
