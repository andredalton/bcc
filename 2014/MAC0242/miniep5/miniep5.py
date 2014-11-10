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
    def __init__(self, transmite_erros=False):
        self.transmite_erros = transmite_erros
        self.ctokens = 0
        self.resposta = None
        self.names = { }
        lex.lex(module=self)
        yacc.yacc(module=self)

    def analisar(self, s):
        self.resposta = None
        self.nao_vazio = False
        yacc.parse(s)
        return self.resposta

    # Definicao dos tokens
    tokens = (
        'NOME','NUMERO',
        'SOM','SUB', 'MUL','DIV','ATRIB',
        )

    def t_SOM(self, t):
        r'\+'
        self.nao_vazio = True
        return t

    def t_SUB(self, t):
        r'-'
        self.nao_vazio = True
        return t

    def t_MUL(self, t):
        r'\*'
        self.nao_vazio = True
        return t

    def t_DIV(self, t):
        r'/'
        self.nao_vazio = True
        return t

    def t_ATRIB(self, t):
        r'='
        self.nao_vazio = True
        return t

    
    def t_NOME(self, t):
        r'[a-zA-Z_][a-zA-Z0-9_]*'
        self.nao_vazio = True
        try:
            self.names[t.value]
        except KeyError:
            self.names[t.value] = None
        return t

    def t_NUMERO(self, t):
        r'\d+\.?\d*[eE][+\-]?\d+|\d+\.?\d*'
        self.nao_vazio = True
        try:
            t.value = float(t.value)
        except ValueError:
            if self.transmite_erros: raise
            print("Por favor, digite apenas numeros validos:", t.value[0])
            t.value = 0
        return t

    # Ignorando tabulacoes
    t_ignore = " \t\v"

    def t_error(self, t):
        if self.transmite_erros: raise TypeError
        print("Caracter incorreto '%s'" % t.value)
        t.lexer.skip(1)

    # Regras de analise sintatica
    precedence = (
        ('right', 'NOME', 'NUMERO'),
        )

    def p_expressao(self, p):
        'statement : expression'
        self.resposta = p[1]

    def p_expressao_dupla(self, p):
        """
        expression : expression expression SOM
                   | expression expression SUB
                   | expression expression MUL
                   | expression expression DIV
                   | NOME expression ATRIB
        """
        if p[1] is not None and p[2] is not None:
            if p[3]   == '=': p[0] = self.names[p[1]] = p[2]
            elif p[3] == '+': p[0] = p[1] + p[2]
            elif p[3] == '-': p[0] = p[1] - p[2]
            elif p[3] == '*': p[0] = p[1] * p[2]
            elif p[3] == '/':
                try:
                    p[0] = p[1] / p[2]
                except ZeroDivisionError:
                    if self.transmite_erros: raise
                    print("Divisao por zero!")

    def p_expressao_dupla2(self, p):
        """
        expression : NOME expression SOM
                   | NOME expression SUB
                   | NOME expression MUL
                   | NOME expression DIV
        """
        if p[1] is not None and p[2] is not None:
            if p[3] == '+': p[0] = self.names[p[1]] + p[2]
            elif p[3] == '-': p[0] = self.names[p[1]] - p[2]
            elif p[3] == '*': p[0] = self.names[p[1]] * p[2]
            elif p[3] == '/':
                try:
                    p[0] = self.names[p[1]] / p[2]
                except ZeroDivisionError:
                    if self.transmite_erros: raise
                    print("Divisao por zero!")


    def p_nome(self, p):
        'expression : NOME'
        p[0] = self.names[p[1]]
        if self.names[p[1]] is None:
            if self.transmite_erros: raise KeyError
            print("Variavel '%s' sem atribuicao" % p[1])

    def p_numero(self, p):
        'expression : NUMERO'
        p[0] = p[1]

    def p_error(self, p):
        if self.nao_vazio:
            if self.transmite_erros: raise LookupError
            print("Erro de sintaxe!", p)

if __name__ == '__main__':
    s = ' '.join(sys.argv[1:])
    f = len(s) != 0
    calc = Calc(f)
    while 1:
        r = calc.analisar(s)
        if r is not None:
            print("%.4g" % r)
            pass
        if f: break
        try:
            s = input('calc > ')
        except (EOFError, KeyboardInterrupt):
            print()
            break