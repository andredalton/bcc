
def main():
    (n, m) = map(int, raw_input().split())

    inf = 10**9+1
    somas = []
    maximos = []

    for i in range(n):
        somas.append(0)
        maximos.append(inf)

    for i in range(m):
        o = map(int, raw_input().split())
        if o[0] == 1:
            for i in range(o[1]-1, o[2]):
                somas[i] += o[3]
                if maximos[i]+o[3] < inf:
                    maximos[i] += o[3]
                else:
                    maximos[i] = inf
        else:
            existe = False
            for i in range(o[1]-1, o[2]):
                if o[3] < maximos[i]:
                    maximos[i] = o[3]
                    existe = True
            if not existe:
                print "NO"
                return

    print "YES"
    for i in range(n):
        if maximos[i] == inf:
            maximos[i] = 42
        print maximos[i] - somas[i],


if __name__ == '__main__':
    main()