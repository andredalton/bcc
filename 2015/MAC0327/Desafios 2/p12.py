# coding=utf-8
__author__ = 'André Meneghelli'

"""
/*******************************************************************************
 * Aluno:  André Meneghelli Vale,      Núm. USP: 4898948
 * Curso: Bacharelado em Ciências da Computação
 * Aula 13 - Bus Routes
 * MAC0327 -- IME/USP, -- Prof. Cristina Gomes Fernandes
 ******************************************************************************/
"""


import sys

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

    if linha in linhas_restantes:
        linhas_restantes.remove(linha)

    for i in xrange(n, len(linhas[linha])-1):
        nova_linha = busca_linha(saidas[linhas[linha][i]])

        if nova_linha is not False:
            bumba(caminhos[linhas[linha][i]][nova_linha], nova_linha)

        saidas[linhas[linha][i]].remove(linha)
        jesus.append(linhas[linha][i])

    for i in xrange(-1, n):
        nova_linha = busca_linha(saidas[linhas[linha][i]])

        if nova_linha is not False:
            bumba(caminhos[linhas[linha][i]][nova_linha], nova_linha)

        saidas[linhas[linha][i]].remove(linha)
        jesus.append(linhas[linha][i])

    if len(linhas_restantes) == 0:
        return True
    return False

def main():
    global saidas
    global linhas_restantes
    global linhas
    global caminhos
    global jesus

    all_input = iter(sys.stdin.read().strip().replace('\n', ' ').split(' '))
    n = int(all_input.next())

    for i in xrange(n):
        linhas_restantes.append(i)
        linhas.append([])
        npontos = int(all_input.next())
        for j in xrange(1, npontos+1):
            ponto = int(all_input.next())
            linhas[len(linhas)-1].append(ponto)
            if caminhos.has_key(ponto):
                if not caminhos[ponto].has_key(i):
                    caminhos[ponto][i] = j-1
            else:
                caminhos[ponto] = {i: j-1}
            if not saidas.has_key(ponto):
                saidas[ponto] = []
            saidas[ponto].append(i)
            j += 1
        all_input.next()

    # Se tiver resposta imprime
    if bumba(0, 0):
        print len(jesus),
        jesus.append(jesus[0])
        print ' '.join(map(str, jesus))
    # Caso contrário imprime zero
    else:
        print 0

if __name__ == '__main__':
    main()