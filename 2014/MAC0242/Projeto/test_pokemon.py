#! /usr/bin/env python3

import os, random, unittest
from pokemon.pokemon import Pokemon

class TestPokemon(unittest.TestCase):
    def setUp(self):
        """ Carrega um pokemon aleatório do billpc."""
        self.p1 = Pokemon()
        self.p2 = Pokemon()
        fname = random.choice(os.listdir("billpc/"))
        f1 = open("billpc/" + fname, "r")
        f2 = open("billpc/" + fname, "r")
        self.p1.load(f1)
        self.p2.load(f2)
        f1.close()
        f2.close()

    def test_distinct_pokemon(self):
        """ Verifica se as duas instâncias são distintas."""
        self.assertNotEqual( self.p1, self.p2 )

    def test_same_kinds(self):
        """ Verifica se as duas instâncias são idênticas."""
        for i in range(2):
            self.assertEqual( self.p1.get_kind(i), self.p2.get_kind(i) )
    
    def test_same_attacks(self):
        """ Verifica se as duas instâncias são idênticas."""
        for i in range(5):
            if self.p1.get_attack(i) is None:
                self.assertEqual( self.p1.get_attack(i), self.p2.get_attack(i) )
            else:
                self.assertEqual( self.p1.get_attack(i).get_TYP(), self.p2.get_attack(i).get_TYP() )

if __name__ == '__main__':
    unittest.main()