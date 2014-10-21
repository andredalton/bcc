#! /usr/bin/env python3

import os, random, unittest
from pokemon.pokemon import Pokemon

class TestPokemon(unittest.TestCase):
    def setUp(self):
        """ Carrega um pokemon aleatório do billpc."""
        self.p1 = Pokemon()
        self.p2 = Pokemon()
        fname = random.choice(os.listdir("billpc/"))
        self.p1.load_file("billpc/" + fname)
        self.p2.load_file("billpc/" + fname)

    def test_distinct_pokemon(self):
        """ Verifica se as duas instâncias são distintas."""
        self.assertNotEqual( self.p1, self.p2 )

    def test_same_kind1(self):
        """ Verifica se as duas instâncias são idênticas."""
        self.assertEqual( self.p1.get_kind1(), self.p2.get_kind1() )
    
    def test_same_kind2(self):
        """ Verifica se as duas instâncias são idênticas."""
        self.assertEqual( self.p1.get_kind2(), self.p2.get_kind2() )

    def test_same_action1(self):
        """ Verifica se as duas instâncias são idênticas."""
        if self.p1.get_action1() is None:
            self.assertEqual( self.p1.get_action1(), self.p2.get_action1() )
        else:
            self.assertEqual( self.p1.get_action1().get_TYP(), self.p2.get_action1().get_TYP() )
    
    def test_same_action2(self):
        """ Verifica se as duas instâncias são idênticas."""
        if self.p1.get_action2() is None:
            self.assertEqual( self.p1.get_action2(), self.p2.get_action2() )
        else:
            self.assertEqual( self.p1.get_action2().get_TYP(), self.p2.get_action2().get_TYP() )
    
    def test_same_action3(self):
        """ Verifica se as duas instâncias são idênticas."""
        if self.p1.get_action3() is None:
            self.assertEqual( self.p1.get_action3(), self.p2.get_action3() )
        else:
            self.assertEqual( self.p1.get_action3().get_TYP(), self.p2.get_action3().get_TYP() )
    
    def test_same_action4(self):
        """ Verifica se as duas instâncias são idênticas."""
        if self.p1.get_action4() is None:
            self.assertEqual( self.p1.get_action4(), self.p2.get_action4() )
        else:
            self.assertEqual( self.p1.get_action4().get_TYP(), self.p2.get_action4().get_TYP() )


if __name__ == '__main__':
    unittest.main()