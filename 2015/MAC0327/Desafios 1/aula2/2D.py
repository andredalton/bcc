# coding=utf-8
__author__ = 'andre'

"""
/*******************************************************************************
 * Aluno:  André Meneghelli Vale,      Núm. USP: 4898948
 * Curso: Bacharelado em Ciências da Computação
 * Aula 2 - Desafio D
 * MAC0327 -- 10/07/2015 -- IME/USP, -- Prof. Cristina Gomes Fernandes
 ******************************************************************************/
"""

def main():
    l = [1, 2, 4]
    print map(lambda (index, v): v + l[index+1], enumerate(l[:-1]))

if __name__ == "__main__":
    main()