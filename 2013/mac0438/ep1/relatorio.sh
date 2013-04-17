#!/bin/bash -  

#Criando todos os binarios para gerar um relatorio com estatística de tempo.
make clean
echo Gerando todos os binarios.
make all

mkdir -p relatorios

PROCESSADOR=`cat /proc/cpuinfo|grep "vendor_id" |cut -d ':' -f 2 | tr -d [:blank:] | head -1`
MODELOPROC=`cat /proc/cpuinfo|grep "model name" |cut -d ':' -f 2 | tr -d [:blank:] | head -1`
CORES=`cat /proc/cpuinfo|grep "cpu cores" |cut -d ':' -f 2 | tr -d [:blank:] | head -1`
CLOCK=`cat /proc/cpuinfo|grep "cpu MHz" |cut -d ':' -f 2 | tr -d [:blank:] | head -1`
CACHE=`cat /proc/cpuinfo|grep "cache size" |cut -d ':' -f 2 | tr -d [:blank:] | head -1`
ARQUITETURA=`uname -m`
FILENAME="$HOSTNAME.$PROCESSADOR"

echo -en "\t$HOSTNAME & $PROCESSADOR & $MODELOPROC & $CORES & $CLOCK & $CACHE & $ARQUITETURA \\" > relatorios/$FILENAME.tex
echo -e "\\ \n\t\\hline" >> relatorios/$FILENAME.tex

echo Gerando relatorio.

echo "" > relatorios/myep1.$FILENAME.tex

for f in `ls entradas/*.txt | cut -d '/' -f 2`
do
	echo Rodando ep1 para \"$f\"
	/usr/bin/time -f "\tep1 & $f & %es & %M Kbytes & %x @@ " -ao relatorios/myep1.$FILENAME.tex ./ep1 entradas/$f > /dev/null
	
	echo Rodando ep1-mili para \"$f\"
	/usr/bin/time -f "\tep1-mili & $f & %es & %M Kbytes & %x @@ " -ao relatorios/myep1.$FILENAME.tex ./ep1-mili entradas/$f > /dev/null
	
	echo Rodando pep1 para \"$f\"
	/usr/bin/time -f "\tpep1 & $f & %es & %M Kbytes & %x @@ " -ao relatorios/myep1.$FILENAME.tex ./pep1 entradas/$f > /dev/null
	
	echo Rodando pep1-mili para \"$f\"
	/usr/bin/time -f "\tpep1-mili & $f & %es & %M Kbytes & %x @@ " -ao relatorios/myep1.$FILENAME.tex ./pep1-mili entradas/$f > /dev/null
	
	echo Rodando gep1 para \"$f\"
	/usr/bin/time -f "\tgep1 & $f & %es & %M Kbytes & %x @@ " -ao relatorios/myep1.$FILENAME.tex ./gep1 entradas/$f > /dev/null
	
	echo Rodando gep1-mili para \"$f\"
	/usr/bin/time -f "\tgep1-mili & $f & %es & %M Kbytes & %x @@ " -ao relatorios/myep1.$FILENAME.tex ./gep1-mili entradas/$f > /dev/null
done

sed -i 's:@@:\\\\\n\t\\hline:g' relatorios/myep1.$FILENAME.tex
sed -i '/^Command exited with non-zero status [0-9]/d' relatorios/ep1.zillertal.GenuineIntel.tex