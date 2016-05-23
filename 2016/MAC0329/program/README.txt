MAC 0329 - Algebra Booleana e Aplicacoes
EP2 - 2015

-------------------------------------------
integrantes:                n.USP:

Antonio Augusto Abello      8536152
Leonardo Daneu Lopes        8516816
Lucas Sung Jun Hong         8124329
William Shinji Numada       7648325
-------------------------------------------
TÓPICOS:

00 - Como usar?
01 - Usando o programa
02 - O que acontece em cada instrução?
03 - O que é esperado com a aplicação do "teste001"






00 - Como usar?
===============

* No arquivo "ep2.circ" em 'main', temos a memória RAM;
* Para inserir a instrução desejada, no teclado, faça CTRL+1 para ativar o botão
  que troca valores nos circuitos ("Change values within circuit (CTRL+1)");
* Selecione assim o endereço desejado e teremos um destaque em vermelho;
* Feito isso, no teclado, digite a instrução desejada.
  Para seguir para próximo endereço, aperte a barra de espaço do teclado;
* Inserido todas as instruções, aperte o botão "CLOCK", que se encontra ao lado do botão "RESET",
  para dar início a um clock, ou faça CTRL+T no teclado.

dicas:
------
* Para rodar o mesmo ciclo de instrução novamente, basta apertar o "RESET";
* Para apagar o ciclo e zerar todos os endereços, clique no botão abaixo do RAM, chamado "clr".

01 - Usando o programa
======================

Temos a seguinte lista de instruções:
(ATENÇÃO: Usaremos o arquivo "teste001" como referência.)

0b05
0c04
3200
3306
0
fade
3200
4600

ou seja, em logisim, teremos os seguintes inputs em RAM:

---+------+----- 
00 | 0b05 | 0c04
---+------+----- 
02 | 3200 | 3306
---+------+-----
04 | 0000 | fade
---+------+-----
06 | 3200 | 4600
---+------+-----


02 - O que acontece em cada instrução?
======================================

 +-----------> instrucão
 |
 |      +----> sequência de execuções
 |      |
 *      *
---------------------------------------------------------------------------------------
0b05    * a instrução {LDA} 05 é carregada no registrador IR;
        * PC é incrementado para apontar para a próxima instrução;
        * instrução carregada em IR é decodificada e executada, 
          sendo o dado da posição 05 ("fade") carregada no Acumulador (ACC);

        teremos:
        +-----+------+
        | IR  | 0b05 |
        +-----+------+
        | ACC | fade |
        +-----+------+

---------------------------------------------------------------------------------------
0c04    * a instrução {STA} 04 é carregada no registrador IR;
        * PC é incrementado para apontar para a próxima instrução;
        * instrução carregada em IR é decodificada e executada, 
          ou seja, o ACC é carregado no endereço 04;

        teremos:
        +-----+------+
        | IR  | 0c04 |
        +-----+------+
        | ACC | fade |
        +-----+------+

        O endereço 05, que contém o dado "fade", foi registrado no Acumulador e
        finalmente carregado no endereço 04.

---------------------------------------------------------------------------------------
3200    * Execução {NOP}, ou seja, sem operação;
        * PC é incrementado para apontar para a próxima instrução;

---------------------------------------------------------------------------------------
3306    * Salto não-condicional para endereço 06;

---------------------------------------------------------------------------------------
0       * instrução ignorada

---------------------------------------------------------------------------------------
0       * instrução ignorada

---------------------------------------------------------------------------------------
3200    * Execução {NOP}, ou seja, sem operação;
        * PC é incrementado para apontar para a próxima instrução;

---------------------------------------------------------------------------------------
4600    * Execução {STP}, ou seja, fim do ciclo de instruções.

---------------------------------------------------------------------------------------

Assim, das entradas:

00 | 0b05 | 0c04
---+------+----- 
02 | 3200 | 3306
---+------+-----
04 | 0000 | fade
---+------+-----
06 | 3200 | 4600
---+------+-----

teremos esse resultado:
00 | 0b05 | 0c04
---+------+----- 
02 | 3200 | 3306
---+------+-----
04 | fade | fade      (temos "fade" no endereço 04, que antes era composto por "0000")
---+------+-----
06 | 3200 | 4600
---+------+-----


03 - O que é esperado com a aplicação do "teste001"
===================================================

Um dado chamado "fade" deve ser lido e armazenado no Acumulador.
Com isso, uma outra instrução carregará o acumulador que contém "fade" no endereço ao lado
onde o "fade" foi lido inicialmente.
O resultado é que "fade" ocorrerá duas vezes seguidas.

No entanto, antes que os dados "fade" ocorram, temos uma instrução "0x33" que pulará as duas
instruções "fade", terminando o ciclo com 0x46.