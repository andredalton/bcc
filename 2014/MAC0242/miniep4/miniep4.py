#! /usr/bin/env python3

import random
import unittest
from math import sin, pi, atan, factorial
from math import cos, sqrt, tan

def sorteio():
    """ Função que sorteia um valor float de 0 a 100."""
    return random.uniform(0, 100)

def fact(n):
    """ Função recursiva para calculo de fatorial."""
    if n == 1:
        return 1
    return n * fact(n-1)

class TestMathFunction(unittest.TestCase):
	""" Função de testes para o módulo matemático do python."""
    def setUp(self):
    	""" Sorteia valores iniciais aleatórios."""
        self.alpha = sorteio()
        self.beta = sorteio()
        self.n = random.randint(0, 20)

    # Testes de Seno
    def test_sin_impar(self):
    	""" Verifica se o seno é uma função ímpar."""
        self.assertEqual( -sin(self.alpha), sin(-self.alpha) )
    def test_sin_a_plus_b(self):
    	""" Verifica a igualdade: sen(a+b) = sen(a)*cos(b) + cos(a)*sen(b)."""
        alpha = random.uniform(0, 100)
        beta = random.uniform(0, 100)
        v1 = sin(self.alpha + self.beta)
        v2 = sin(self.alpha)*cos(self.beta) + sin(self.beta)*cos(self.alpha)
        self.assertAlmostEqual( v1, v2 )
    def test_sin_pitagoras(self):
    	""" Verifica a igualdade de pitágoras."""
        self.assertAlmostEqual( pow(sin(self.alpha),2) + pow(cos(self.alpha),2), 1 )

    # Testes de fatorial
    def test_factorial_x_fact(self):
    	""" Compara a função math.factorial com a função recursiva fact."""
        self.assertEqual(factorial(self.n), fact(self.n))
    def test_factorial_negativo(self):
    	""" Verifica se ocorre exceção para valores negativos."""
        self.assertRaises(ValueError, factorial, (-1))
    def test_factorial_div(self):
    	""" Verifica a igualdade: n!/(n-1)! = n."""
        self.assertEqual(factorial(self.n)/factorial(self.n-1), self.n)

    # Testes de pi
    def test_pi_sen(self):
    	""" Verifica se sen(pi) = 0."""
        self.assertAlmostEqual( sin(pi), 0)
    def test_pi_cos(self):
    	""" Verifica se cos(pi/2) = 0."""
        self.assertAlmostEqual( cos(pi/2), 0)
    def test_pi_tan(self):
    	""" Verifica se tan(pi) = 0."""
        self.assertAlmostEqual( tan(pi), 0)

    # Testes de atan
    def test_atan_impar(self):
    	""" Verifica se atan é uma fun;cão ímpar."""
        self.assertEqual( -atan(self.alpha), atan(-self.alpha) )
    def test_atan_1_per_x(self):
    	""" Verifica a igualdade de atan(1/x) = pi/2-atan(x) para x positivo."""
        self.assertAlmostEqual(atan(1/self.alpha), pi/2-atan(self.alpha))
    def test_atan_minus_1_per_x(self):
    	""" Verifica a igualdade de atan(1/x) = -pi/2-atan(x) para x negativo."""
        self.assertAlmostEqual(atan(-1/self.alpha), -pi/2-atan(-self.alpha))
    def test_atan_0(self):
    	""" Verifica se atan(0) = 0."""
        self.assertAlmostEqual( atan(0), 0)
    def test_atan_inf(self):
    	""" Verifica se atan(inf) = pi/2."""
        self.assertAlmostEqual( atan(float("inf")), pi/2)


if __name__ == '__main__':
    unittest.main()