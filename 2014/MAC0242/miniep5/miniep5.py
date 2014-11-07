#!/usr/bin/env python3

# Apenas para poder manipular a linha de comando
import sys

# Manipulacao do historico de comandos para facilitar os testes.
import readline
histfile = ".history"
try:
    readline.read_history_file(histfile)
except IOError:
    pass
import atexit
atexit.register(readline.write_history_file, histfile)
del histfile

# Importando o analizador lexico e sintático
import ply.lex as lex
import ply.yacc as yacc
    
class Calc:
    def __init__(self):
        self.names = { }
        lex.lex(module=self)
        yacc.yacc(module=self)

    def analisar(self, str):
        return yacc.parse(str)
        

    # Definicao dos tokens
    tokens = ( 'NOME','NUMERO', 'SOM','SUB', 'MUL','DIV','ATRIB' )

    t_SOM     = r'\+'
    t_SUB     = r'-'
    t_MUL     = r'\*'
    t_DIV     = r'/'
    t_ATRIB   = r'='
    t_NOME    = r'[a-zA-Z_][a-zA-Z0-9_]*'

    def t_NUMERO(self, t):
        r'\d+\.?\d*[eE][+\-]?\d+|\d+\.?\d*'
        try:
            t.value = float(t.value)
        except ValueError:
            print("Por favor, digite apenas numeros validos:", t.value[0])
            t.value = 0
        return t

    # Ignorando tabulacoes
    t_ignore = " \t"

    def t_error(self, t):
        print("Caracter incorreto '%s'" % t.value)
        t.lexer.skip(1)

    # Regras de analise sintatica
    precedence = (
        ('left','SOM','SUB'),
        ('left','MUL','DIV')
        )

    def p_atribuicao(self, p):
        'statement : NOME ATRIB expression'
        self.names[p[1]] = p[3]

    def p_expressao(self, p):
        'statement : expression'
        print(p[1])

    def p_expressao_dupla(self, p):
        """
        expression : expression expression SOM
                  | expression expression SUB
                  | expression expression MUL
                  | expression expression DIV
        """
        if p[1] is None or p[2] is None:
            p[0] = None
        elif p[3] == '+'  : p[0] = p[1] + p[2]
        elif p[3] == '-': p[0] = p[1] - p[2]
        elif p[3] == '*': p[0] = p[1] * p[2]
        elif p[3] == '/':
            try:
                p[0] = p[1] / p[2]
            except ZeroDivisionError:
                print("Divisao por zero!")


    def p_numero(self, p):
        'expression : NUMERO'
        p[0] = p[1]

    def p_nome(self, p):
        'expression : NOME'
        try:
            p[0] = self.names[p[1]]
        except LookupError as e:
            print("Variavel '%s' sem atribuicao" % p[1])

    def p_error(self, p):
        print("Erro de sintaxe!")

if __name__ == '__main__':
    s = ' '.join(sys.argv[1:])
    f = len(s) != 0
    calc = Calc()
    while 1:
        if len(s) > 0:
            calc.analisar(s)
        if f: break
        try:
            s = input('calc > ')
        except (EOFError, KeyboardInterrupt):
            print()
            break