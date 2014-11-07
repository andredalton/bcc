#! /usr/bin/env python3

import os, random, unittest, subprocess

def run(s):
    return  subprocess.getoutput("./miniep5.py " + s ).replace("Generating LALR tables\n", "")

def gera_var():
    p = ""
    for i in range(random.randint(1,10)):
        p += random.choice(["a", "e", "i", "o", "u"])
    return p

def gera_teste(num, op):
    lst = []
    i = 0
    j = 0
    while i < num and j < op:
        r = random.random()
        if i > j + 1 and r < 0.6:
            lst.append( random.choice(["+", "-", "*", "/"]) )
            j += 1
        lst.append( str(random.random()*10**random.randint(-30,30)) )
        i += 1
    for j in range(j, op):
        lst.append( random.choice(["+", "-", "*", "/"]) )
    return " ".join(["\""] + lst + ["\""] )
    
class Test(unittest.TestCase):
    # Testando inf, -inf e nan
    def test_inf(self):
        """ Verifica a comutatividade da soma."""
        out = run("1" * 1000)
        self.assertEqual( out, "inf" )
    
    def test_minf(self):
        """ Verifica a comutatividade da soma."""
        out = run(' '.join(["0", "1" * 1000, "-"]))
        self.assertEqual( out, "-inf" )

    def test_nan(self):
        """ Verifica a comutatividade da soma."""
        out = run(' '.join(["1" * 1000, "0", "1" * 1000, "- +"]))
        self.assertEqual( out, "nan" )

    # Testando propriedades básicas das operações.
    def test_som_comutativa(self):
        """ Verifica a comutatividade da soma."""
        a = random.random()
        b = random.random()
        aout = run(' '.join(["\"", str(a), str(b), "+\""]))
        bout = run(' '.join(["\"", str(b), str(a), "+\""]))
        self.assertEqual( aout, bout )

    def test_som_elemento_neutro(self):
        """ Verifica o elemento neutro da soma."""
        a = random.random()
        aout = run(' '.join(["\"", str(a), "0 +\""]))
        self.assertEqual( a, float(aout) )

    def test_som_anulamento(self):
        """ Verifica o anulamento da soma."""
        a = random.random()
        aout = run(' '.join(["\"", str(a), "0", str(a)," - +\""]))
        self.assertEqual( 0, float(aout) )

    def test_sub_comutativa(self):
        """ Verifica a não comutatividade da subtração."""
        a = random.random()
        b = random.random()
        aout = run(' '.join(["\"", str(a), str(b), "-\""]))
        bout = run(' '.join(["\"", str(b), str(a), "-\""]))
        self.assertNotEqual( aout, bout )

    def test_sub_elemento_neutro(self):
        """ Verifica o elemento neutro da soma."""
        a = random.random()
        aout = run(' '.join(["\"", str(a), "0 -\""]))
        self.assertEqual( a, float(aout) )

    def test_som_anulamento(self):
        """ Verifica o anulamento da soma."""
        a = random.random()
        aout = run(' '.join(["\"", str(a), str(a)," -\""]))
        self.assertEqual( 0, float(aout) )

    def test_mul_comutativa(self):
        """ Verifica a comutatividade da multiplicacao."""
        a = random.random()
        b = random.random()
        aout = run(' '.join(["\"", str(a), str(b), "*\""]))
        bout = run(' '.join(["\"", str(b), str(a), "*\""]))
        self.assertEqual( aout, bout )

    def test_mul_elemento_neutro(self):
        """ Verifica o elemento neutro da multiplicacao."""
        a = random.random()
        aout = run(' '.join(["\"", str(a), "1 *\""]))
        self.assertEqual( a, float(aout) )

    def test_mul_elemento_nulo(self):
        """ Verifica o elemento nulo da multiplicacao."""
        a = random.random()
        aout = run(' '.join(["\"", str(a), "0 *\""]))
        self.assertEqual( 0, float(aout) )

    def test_div_comutativa(self):
        """ Verifica a não comutatividade da divisão."""
        a = random.random()
        b = random.random()
        aout = run(' '.join(["\"", str(a), str(b), "/\""]))
        bout = run(' '.join(["\"", str(b), str(a), "/\""]))
        self.assertNotEqual( aout, bout )

    def test_div_elemento_neutro(self):
        """ Verifica o elemento neutro da divisão."""
        a = random.random()
        aout = run(' '.join(["\"", str(a), "1 /\""]))
        self.assertEqual( a, float(aout) )

    def test_div_zero(self):
        """ Verifica a divisao por zero."""
        a = random.random()
        aout = run(' '.join(["\"", str(a), "0 /\""]))
        self.assertEqual( aout, "Divisao por zero!\nNone" )

    # Testes de sintaxe.
    def test_sintaxe(self):
        """
        Verifica sintaxe quando existem numeros = operações - 1.
        Queria comparar com o não erro, mas não encontrei a maneira adequada de se fazer isso.
        """
        n = random.randint(1, 1000)
        s = gera_teste(n, n-1)
        try:
            out = float(run(s))
            conv = True
        except ValueError:
            conv = False
        self.assertTrue(conv)

    def test_erro_sintaxe1(self):
        """ Verifica erros de sintaxe quando existem mais numeros do que operações - 1."""
        n = random.randint(1, 1000)
        s = gera_teste(n + random.randint(1, 100), n-1)
        out = run(s)
        self.assertRaises(ValueError, float, out)

    def test_erro_sintaxe1(self):
        """ Verifica erros de sintaxe quando existem menos numeros do que operações - 1."""
        n = random.randint(1, 1000)
        s = gera_teste(n, n + random.randint(0, 100))
        out = run(s)
        self.assertRaises(ValueError, float, out)

    def test_caracter_incorreto(self):
        """ Verifica a ocorrencia de erro quando se utiliza um caracter não especificado."""
        c = random.choice(["!", "@", "$", "?"])
        out = run( c )
        self.assertEqual( out, "Caracter incorreto '%c'\nErro de sintaxe!" % c)

    def test_variavel_nao_inicializada(self):
        """ Verifica a ocorrencia de erro quando se utiliza um caracter não especificado."""
        s = gera_var()
        out = run( s )
        self.assertEqual( out, "Variavel '%s' sem atribuicao\nNone" % s)

if __name__ == '__main__':
    unittest.main()