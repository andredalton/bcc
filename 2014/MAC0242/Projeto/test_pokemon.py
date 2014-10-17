#! /usr/bin/env python3

import os, random, unittest
from pokemon import Pokemon

class TestPokemon(unittest.TestCase):
    def setUp(self):
        """ Carrega um pokemon aleatório do billpc."""
        self.p1 = Pokemon()
        self.p2 = Pokemon()
        fname = random.choice(os.listdir("billpc/"))
        self.p1.carrega_arquivo("billpc/" + fname)
        self.p2.carrega_arquivo("billpc/" + fname)

    def test_distinct_pokemon(self):
        """ Verifica se as duas instâncias são distintas."""
        self.assertNotEqual( self.p1, self.p2 )

    def test_same_tipo1(self):
        """ Verifica se as duas instâncias são idênticas."""
        self.assertEqual( self.p1.get_tipo1(), self.p2.get_tipo1() )
    
    def test_same_tipo2(self):
        """ Verifica se as duas instâncias são idênticas."""
        self.assertEqual( self.p1.get_tipo2(), self.p2.get_tipo2() )

    def test_same_acao1(self):
        """ Verifica se as duas instâncias são idênticas."""
        self.assertEqual( self.p1.get_acao1(), self.p2.get_acao1() )
    
    def test_same_acao2(self):
        """ Verifica se as duas instâncias são idênticas."""
        self.assertEqual( self.p1.get_acao2(), self.p2.get_acao2() )
    
    def test_same_acao3(self):
        """ Verifica se as duas instâncias são idênticas."""
        self.assertEqual( self.p1.get_acao3(), self.p2.get_acao3() )
    
    def test_same_acao4(self):
        """ Verifica se as duas instâncias são idênticas."""
        self.assertEqual( self.p1.get_acao4(), self.p2.get_acao4() )


if __name__ == '__main__':
    unittest.main()

