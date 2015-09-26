# coding=utf-8
__author__ = 'avale'

from pprint import pprint

saidas = {}             # Guarda as linhas pelas quais se deixa um ponto
linhas_restantes = []   # Linhas ainda não utilizadas
linhas = []             # O caminho de cada uma das linhas
caminhos = {}           # Uma matriz que dado um ponto, diz qual o indice dele em cada uma das linhas
jesus = []              # Jesus é o caminho!

# Função de busca binária convencional
def busca_bin(x, v):
    e = 0
    d = len(v)-1
    while e <= d:
        m = (e + d)/2
        if v[m] == x:
            return True
        if v[m] < x:
            e = m + 1
        else:
            d = m - 1
    return False

# Função que usa a busca binaria pra definir se existe alguma linha não visitada neste ponto
def busca_linha(v):
    global linhas_restantes
    for l in linhas_restantes:
        if busca_bin(l, v):
            return l
    return False

# É onde está o bagulho todo!
def bumba(n, linha):
    global saidas
    global linhas_restantes
    global linhas
    global caminhos
    global jesus

    # print "\n----------\nAcessando a linha", linha, "no ponto", n

    if linha in linhas_restantes:
        linhas_restantes.remove(linha)

    for i in xrange(n, len(linhas[linha])-1):
        nova_linha = busca_linha(saidas[linhas[linha][i]])

        if nova_linha is not False:
            # print "-", linhas[linha][i], nova_linha, caminhos[linhas[linha][i]][nova_linha]
            bumba(caminhos[linhas[linha][i]][nova_linha], nova_linha)
        # else:
        #     print "-", linhas[linha][i], nova_linha

        saidas[linhas[linha][i]].remove(linha)
        jesus.append(linhas[linha][i])

    for i in xrange(-1, n):
        nova_linha = busca_linha(saidas[linhas[linha][i]])

        if nova_linha is not False:
            # print "*", linhas[linha][i], nova_linha, caminhos[linhas[linha][i]][nova_linha]
            bumba(caminhos[linhas[linha][i]][nova_linha], nova_linha)
        # else:
        #     print "*", linhas[linha][i], nova_linha


        saidas[linhas[linha][i]].remove(linha)
        jesus.append(linhas[linha][i])

    if len(linhas_restantes) == 0:
        return True
    return False
    # print linhas[linha][n], "",
    # acessos[linhas[linha][n]].remove(linha)


def main():
    global saidas
    global linhas_restantes
    global linhas
    global caminhos
    global jesus
    n = int(raw_input())

    # Inicializando as variaveis globais
    for i in xrange(n):
        linha = map(int, raw_input().split())
        linhas_restantes.append(i)
        linhas.append(linha[1:len(linha)-1])
        for j in xrange(1, linha[0]+1):
            if caminhos.has_key(linha[j]):
                if not caminhos[linha[j]].has_key(i):
                    caminhos[linha[j]][i] = j-1
            else:
                caminhos[linha[j]] = {i: j-1}
            if not saidas.has_key(linha[j]):
                saidas[linha[j]] = []
            saidas[linha[j]].append(i)

    # Aqui é só pra dar uma olhada nas variáveis
    # print "saidas"
    # pprint(saidas)
    # print "\nlinhas"
    # pprint(linhas)
    # print "\nTotal arestas"
    # print sum(map(len, linhas))
    # print "\nlinhas_restantes"
    # pprint(linhas_restantes)
    # print "\ncaminhos"
    # pprint(caminhos)
    # print "\n"

    # Se tiver resposta imprime
    if bumba(0, 0):
        print len(jesus),
        jesus.append(jesus[0])
        print ' '.join(map(str, jesus))
    # Caso contrário imprime zero
    else:
        print 0

    # só pra verificar se todas as arestas foram utilizadas
    # print "\nsaidas"
    # pprint(saidas)

    return

if __name__ == '__main__':
    main()