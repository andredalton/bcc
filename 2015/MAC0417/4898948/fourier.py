# coding=utf-8
import argparse
import numpy as np
import matplotlib.pyplot as plt
from matplotlib.patches import Polygon

def complex2tuple(points):
    """
    Função auxiliar que transforma uma lista de números complexos em uma lista de tuplas no plano cartesiano.
    :param points: Lista de números complexos
    :return: Retorna a lista de tuplas seguida dos limites de cada eixo.
    """
    reals = []
    maxV = minV = minH = maxH = None
    for p in points:
        if maxV is None:
            maxV = minV = p.imag
            maxH = minH = p.real
        reals.append((p.real, p.imag))
        if p.real < minH:
            minH = p.real
        elif p.real > maxH:
            maxH = p.real
        if p.imag < minV:
            minV = p.imag
        elif p.imag > maxV:
            maxV = p.imag
    return reals, ([minH-1, maxH+1], [minV-1, maxV+1])

def reconstroi(descritores):
    points = np.fft.ifft(descritores)
    rpoints, limits = complex2tuple(points)
    fig = plt.figure()
    ax = fig.add_subplot(111)
    ax.add_patch(Polygon(rpoints, closed=True, fill=False, hatch='-|'))
    ax.set_xlim(limits[0])
    ax.set_ylim(limits[1])
    plt.show()
    return points

def calcula_descritores(points):
    return np.fft.fft(points)

def main():
    parser = argparse.ArgumentParser(description='Cálculo dos descritores de Fourier.')
    parser.add_argument('-p', '--points', metavar='"points"', type=str, required=True, help='Vetor de pontos complexos.')
    parser.add_argument('--print', dest='out', action='store_true')
    parser.set_defaults(out=False)
    args = parser.parse_args()
    points = eval(args.points)
    descritores = calcula_descritores(points)
    if args.out:
        reconstroi(descritores)
    else:
        print descritores

if __name__ == "__main__":
    main()
