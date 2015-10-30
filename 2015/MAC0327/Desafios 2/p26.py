# coding=utf-8
__author__ = 'André Meneghelli'

"""
/*******************************************************************************
 * Aluno:  André Meneghelli Vale,      Núm. USP: 4898948
 * Curso: Bacharelado em Ciências da Computação
 * Aula 18 - Ouths and Crosses
 * MAC0327 -- IME/USP, -- Prof. Cristina Gomes Fernandes
 ******************************************************************************/
"""


# Define o vencedor.
def vencedor(soma, profundidade):
    if soma == 3:
        return 4 - profundidade
    elif soma == -3:
        return profundidade - 4
    else:
        return 0


# Define o estado atual da partida
def estado(cross, profundidade):
    lin = [0, 0, 0]
    col = [0, 0, 0]
    dia = [0, 0]
    for i in xrange(3):
        for j in xrange(3):
            lin[i] += cross[i*3+j]
            col[i] += cross[j*3+i]
            if i == j:
                dia[0] += cross[i*3+j]
            if i + j == 2:
                dia[1] += cross[i*3+j]

    if vencedor(dia[0], profundidade):
        return vencedor(dia[0], profundidade)
    if vencedor(dia[1], profundidade):
        return vencedor(dia[1], profundidade)
    for i in xrange(3):
        if vencedor(lin[i], profundidade):
            return vencedor(lin[i], profundidade)
        if vencedor(col[i], profundidade):
            return vencedor(col[i], profundidade)
    return 0


# Realiza uma jogada
def joga(cross, jogador=1, profundidade=0):
    atual = estado(cross, profundidade)
    if atual or (0 not in cross):
        return atual

    profundidade += 1
    jogadas = []
    for i in xrange(len(cross)):
        if cross[i] == 0:
            cross[i] = jogador
            jogadas.append(joga(cross, -1*jogador, profundidade))
            cross[i] = 0

    if jogador == 1:
        return max(jogadas)
    return min(jogadas)


if __name__ == "__main__":
    cross = []
    i = 0
    while i < 9:
        buff = raw_input()
        for c in buff:
            if c == "X":
                cross.append(1)
            elif c == "#":
                cross.append(0)
            else:
                cross.append(-1)
            i += 1

    resultado = joga(cross)

    if resultado > 0:
        print "Crosses win"
    elif resultado < 0:
        print "Ouths win"
    else:
        print "Draw"