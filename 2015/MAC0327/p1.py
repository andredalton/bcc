__author__ = 'avale'

from math import sqrt
# from pprint import pprint

def  main():
    n = int(raw_input())
    cidades = []
    distancias = {}

    for i in range(n):
        cidades.append(map(int, raw_input().split()))

    for i in range(n):
        for j in range(i+1, n):
            # print cidades[i], cidades[j]
            if (cidades[i][0] - cidades[j][0]) != 0:
                a = 1.0*(cidades[i][1] - cidades[j][1]) / (cidades[i][0] - cidades[j][0])
            else:
                a = "inf"
            if a != "inf":
                b = cidades[i][1] - cidades[i][0]*a
            else:
                b = cidades[i][0]
                # b = cidades[i][1]
            d = sqrt((cidades[i][0] - cidades[j][0])**2 + (cidades[i][1] - cidades[j][1])**2)
            # print "----------------------------------"
            # print (cidades[i][0] - cidades[j][0])**2, (cidades[i][1] - cidades[j][1])**2
            # print (cidades[i][0] - cidades[j][0])**2 + (cidades[i][1] - cidades[j][1])**2
            # print sqrt((cidades[i][0] - cidades[j][0])**2 + (cidades[i][1] - cidades[j][1])**2)
            # print d, "\n----------------------------------"

            try:
                if distancias[(a,b)] < d:
                    distancias[(a,b)] = d
            except KeyError:
                distancias[(a,b)] = d
            # print a, b, d, "\n================================="

    # print cidades
    # print "\n\n\n"
    # pprint(distancias)
    # print "\n\n\n"
    print int(round(sum(distancias.values())))
    # print int(sum(map(round, distancias.values())))

if __name__ == '__main__':
    main()