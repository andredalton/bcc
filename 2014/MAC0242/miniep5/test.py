#! /usr/bin/env python3

import os, random, unittest, subprocess

from miniep5 import Calc

def gera_var():
    """ Funcao que gera um nome de variavel aleatorio com 1 a 10 caracteres. """
    p = ""
    for i in range(random.randint(1,10)):
        p += random.choice(["a", "e", "i", "o", "u"])
    return p

def gera_teste(num, op, vr=0):
    """
    Funcao que gera um teste aleatorio onde:
    num: Numeros de floats e variaveis gerado
    op:  Numero de operadores gerados
    vr:  Probabilidade de inserir uma nova variavel
    """
    lst = []
    var = []
    var_atrr = set()
    i = 0                   # Numero de floats e variáveis
    j = 0                   # Numero de operadores
    p = random.random()     # Probabilidade de um operador ser inserido no início da expressao
    pws = random.random()   # Probabilidade de insercao de espacos em branco
    pvr = vr                # Probabilidade de ocorrencia de variáveis
    patr = random.random()  # Probabilidade de atribuição em uma variável.
    tokens = ["+", "-", "*", "/"]
    while i < num:
        r = random.random()
        if r < pws:
            # Inserindo espacos em branco
            lst.append( random.choice([" ", "\t"]) * random.randint(1, 30) )
        if r < patr:
            if len(var) > 0 and var[-1]['num']==1:
                # Atribuindo a uma variavel
                v = var.pop()
                var_atrr.add(v['nome'])
                lst.append("=")
                if len(var)>0:
                    var[-1]['num'] += 1
                j += 1
            elif i > j + 1 + len(var) + len(var_atrr):
                # Inserindo um operador.
                if len(var) == 0 or ( len(var)>0 and var[-1]['num']>0 ):
                    if len(var) > 0:
                        var[-1]['num'] -= 1
                    lst.append( random.choice(tokens) )
                    j += 1
        if i < num-1 and r < pvr:
            # Inserindo uma variavel.
            v = gera_var()
            var.append({'nome': v, 'num': 0})
            lst.append(v)
        else:
            # Inserindo numero
            if len(var) > 0:
                var[-1]['num'] += 1
            lst.append( str(random.random()*10**random.randint(-30,30)) )
        i += 1
    while len(var)>0:
        if var[-1]['num'] <= 1:
            var.pop()
            lst.append("=")
            if len(var) > 0:
                var[-1]['num'] += 1    
        else:
            lst.append( random.choice(tokens) )
            var[-1]['num'] -= 1
        j += 1
    for j in range(j, op):
        lst.append( random.choice(tokens) )
    return " ".join(lst)

class Test(unittest.TestCase):
    def setUp(self):
        """ Inicializa a calculadora."""
        self.calc = Calc(True)

    # Testando inf, -inf e nan
    def test_inf(self):
        """ Verifica a ocorrencia do inf."""
        inf = self.calc.analisar("1" * 1000)
        self.assertEqual( inf, float("inf") )
    
    def test_minf(self):
        """ Verifica a ocorrencia do -inf."""
        minf = self.calc.analisar("0 %s -" % ("1"*1000))
        self.assertEqual( minf, float("-inf") )

    def test_nan(self):
        """ Verifica a ocorrencia do nan."""
        out = self.calc.analisar("%(b)s 0 %(b)s - +" % {'b': "1"*1000})
        self.assertNotEqual( out, out )

    # Testando propriedades básicas das operações.
    def test_som_comutativa(self):
        """ Verifica a comutatividade da soma."""
        dic = {'a': random.random(), 'b': random.random()}
        aout = self.calc.analisar("%(a)f %(b)f +" % dic)
        bout = self.calc.analisar("%(b)f %(a)f +" % dic)
        self.assertEqual( aout, bout )

    def test_som_elemento_neutro(self):
        """ Verifica o elemento neutro da soma."""
        a = random.random()
        aout = self.calc.analisar("%.100f 0 +" % a)
        self.assertEqual( a, aout )

    def test_som_anulamento(self):
        """ Verifica o anulamento da soma."""
        dic = {'a': random.random()}
        aout = self.calc.analisar("%(a)f 0 %(a)f - +" %dic)
        self.assertEqual( 0, aout )

    def test_sub_comutativa(self):
        """ Verifica a não comutatividade da subtração."""
        dic = {'a': random.random(), 'b': random.random()}
        aout = self.calc.analisar("%(a)f %(b)f -" % dic)
        bout = self.calc.analisar("%(b)f %(a)f -" % dic)
        self.assertNotEqual( aout, bout )

    def test_sub_elemento_neutro(self):
        """ Verifica o elemento neutro da subtração."""
        a = random.random()
        aout = self.calc.analisar("%.100f 0 -" % a)
        self.assertEqual( a, aout )

    def test_sub_anulamento(self):
        """ Verifica o anulamento da subtração."""
        dic = {'a': random.random()}
        aout = self.calc.analisar("%(a)f %(a)f -" % dic)
        self.assertEqual( 0, aout )

    def test_mul_comutativa(self):
        """ Verifica a comutatividade da multiplicacao."""
        dic = {'a': random.random(), 'b': random.random()}
        aout = self.calc.analisar("%(a)f %(b)f *" % dic)
        bout = self.calc.analisar("%(a)f %(b)f *" % dic)
        self.assertEqual( aout, bout )

    def test_mul_elemento_neutro(self):
        """ Verifica o elemento neutro da multiplicacao."""
        a = random.random()
        aout = self.calc.analisar("%.100f 1 *" % a)
        self.assertEqual( a, aout )

    def test_mul_elemento_nulo(self):
        """ Verifica o elemento nulo da multiplicacao."""
        aout = self.calc.analisar("%.100f 0 *" % random.random())
        self.assertEqual( 0, aout )

    def test_div_comutativa(self):
        """ Verifica a não comutatividade da divisão."""
        dic = {'a': random.random(), 'b': random.random()}
        aout = self.calc.analisar("%(a)f %(b)f /" % dic)
        bout = self.calc.analisar("%(b)f %(a)f /" % dic)
        self.assertNotEqual( aout, bout )

    def test_div_elemento_neutro(self):
        """ Verifica o elemento neutro da divisão."""
        a = random.random()        
        aout = self.calc.analisar("%.100f 1 /" % a)
        self.assertEqual( a, aout )

    def test_div_zero(self):
        """ Verifica a divisao por zero."""
        a = random.random()
        self.assertRaises(ZeroDivisionError, self.calc.analisar, "%.100f 0 /" % a)

    # Testes de sintaxe.
    def test_sintaxe(self):
        """
        Verifica sintaxe quando existem numeros = operações - 1.
        Queria comparar com o não erro, mas não encontrei a maneira adequada de se fazer isso.
        """
        n = random.randint(1, 10000)
        s = gera_teste(n, n-1)
        try:            
            out = float(self.calc.analisar(s))
            conv = True
        except ValueError:
            conv = False
        self.assertTrue(conv)

    def test_erro_sintaxe1(self):
        """ Verifica erros de sintaxe quando existem mais numeros do que operações - 1."""
        n = random.randint(1, 10000)
        s = gera_teste(n + random.randint(2, 100), n)
        self.assertRaises(LookupError, self.calc.analisar, s)

    def test_erro_sintaxe2(self):
        """ Verifica erros de sintaxe quando existem menos numeros do que operações - 1."""
        n = random.randint(1, 10000)
        s = gera_teste(n, n + random.randint(0, 100))
        self.assertRaises(LookupError, self.calc.analisar, s)

    def test_caracter_incorreto(self):
        """ Verifica a ocorrencia de erro quando se utiliza um caracter não especificado."""
        self.assertRaises(TypeError, self.calc.analisar, random.choice(["!", "@", "$", "?"]) )

    # Teste com variaveis.
    def test_variavel_nao_inicializada(self):
        """ Verifica a ocorrencia de erro quando se utiliza uma variável não inicializa."""
        self.assertRaises(KeyError, self.calc.analisar, gera_var())

    def test_sintaxe_atribuicao(self):
        """
        Verifica sintaxe quando existem numeros + variaveis = operações - 1.
        Queria comparar com o não erro, mas não encontrei a maneira adequada de se fazer isso.
        """
        n = random.randint(1, 10000)
        s = gera_teste(n, n-1, 0.3)
        try:            
            out = float(self.calc.analisar(s))
            conv = True
        except ValueError:
            conv = False
        self.assertTrue(conv)

    def test_atribuicao(self):
        """ Verifica a ocorrencia de erro ao se atribuir o valor de uma variavel e reutiliza-lo na mesma expressao."""
        a = random.random()
        dic = {'a': a, 'v': gera_var()}
        self.assertEqual(a*a+a, self.calc.analisar("%(v)s %(a).100f = %(v)s %(v)s * +" % dic))

if __name__ == '__main__':
    unittest.main()