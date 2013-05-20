#!/bin/bash -  

#/**************
# * relatorio.sh
# *************/
#/******************************************************************************
# * Alunos: André Meneghelli Vale,      Núm. USP: 4898948
# *         Marcello Souza de Oliveira, Núm. USP: 6432692
# * Curso: Bacharelado em Ciências da Computação
# * EP 01 -- Simulação Concorrente de uma prova de 'Iron Man'
# * MAC0438 -- 17/04/2013 -- IME/USP, -- Prof. Daniel Macêdo Batista
# * Compilador: gcc linux 4.6.3
# * Editor: Sublime Text 2;
# * Sistema Operacional: Linux
# *****************************************************************************/

#Criando todos os binarios para gerar um relatorio com estatística de tempo.
make clean
echo Gerando todos os binarios.
make all

PROCESSADOR=`cat /proc/cpuinfo|grep "vendor_id" |cut -d ':' -f 2 | tr -d [:blank:] | head -1`
MODELOPROC=`cat /proc/cpuinfo|grep "model name" |cut -d ':' -f 2 | tr -d [:blank:] | head -1`
CORES=`cat /proc/cpuinfo|grep "cpu cores" |cut -d ':' -f 2 | tr -d [:blank:] | head -1`
CLOCK=`cat /proc/cpuinfo|grep "cpu MHz" |cut -d ':' -f 2 | tr -d [:blank:] | head -1`
CACHE=`cat /proc/cpuinfo|grep "cache size" |cut -d ':' -f 2 | tr -d [:blank:] | head -1`
ARQUITETURA=`uname -m`
FILENAME="$HOSTNAME.$PROCESSADOR"

echo -en "\t$HOSTNAME & $MODELOPROC & $CORES & $CLOCK & $CACHE & $ARQUITETURA \\" > relatorios/mycpu.tex
echo -e "\\ \n\t\\hline" >> relatorios/mycpu.tex

echo Gerando relatorio.

echo "" > relatorios/myep1.tex


for precisao in 0.001 0.0000001 0.0000000000000001 0.0000000000000000000000001 0.0000000000000000000000000000000000000000001
do
	for modo in "SEQUENCIAL" "UNLIMITED" "NORMAL"
	do
		echo Rodando ep2 para \"$precisao\"
		/usr/bin/time -f "\t$HOSTNAME & $precisao & $modo & %es & %M Kbytes & %x @@ " -ao relatorios/myep2.tex ./ep2 $precisao > /dev/null
	done
done



#echo Rodando ep1-mili para \"$f\"
#/usr/bin/time -f "\t$HOSTNAME & ep1-mili & $f & %es & %M Kbytes & %x @@ " -ao relatorios/myep1.tex ./ep1-mili entradas/$f > /dev/null
#
#echo Rodando pep1 para \"$f\"
#/usr/bin/time -f "\t$HOSTNAME & pep1 & $f & %es & %M Kbytes & %x @@ " -ao relatorios/myep1.tex ./pep1 entradas/$f > /dev/null
#
#echo Rodando pep1-mili para \"$f\"
#/usr/bin/time -f "\t$HOSTNAME & pep1-mili & $f & %es & %M Kbytes & %x @@ " -ao relatorios/myep1.tex ./pep1-mili entradas/$f > /dev/null
#
#echo Rodando gep1 para \"$f\"
#/usr/bin/time -f "\t$HOSTNAME & gep1 & $f & %es & %M Kbytes & %x @@ " -ao relatorios/myep1.tex ./gep1 entradas/$f > /dev/null
#
#echo Rodando gep1-mili para \"$f\"
#/usr/bin/time -f "\t$HOSTNAME & gep1-mili & $f & %es & %M Kbytes & %x @@ " -ao relatorios/myep1.tex ./gep1-mili entradas/$f > /dev/null
#
#sed -i 's:@@:\\\\\n\t\\hline:g' relatorios/myep1.tex
#sed -i 's:_:\\_:g' relatorios/myep1.tex
#sed -i 's:_:\\_:g' relatorios/mycpu.tex
#sed -i '/^Command exited with non-zero status [0-9]/d' relatorios/myep1.tex
#
#cat relatorios/ep1.*.tex > relatorios/tempos.tex
#cat relatorios/cpu.*.tex > relatorios/maquinas.tex
#
#cd relatorios/
#pdflatex relatorio.tex