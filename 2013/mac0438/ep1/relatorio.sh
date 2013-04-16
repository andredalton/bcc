#!/bin/bash -  

Criando todos os binarios para gerar um relatorio com estatística de tempo.
make clean
make all

echo Gerando relatorio.

echo "Relatorio gerado automaticamente." > relatorio.txt

for f in entradas/*.txt
do
	if [ "$f" != "relatorio.txt" ]; then
		/usr/bin/time -f "\n\n$f\n\nep1\n%es\n%MKb" -ao relatorio.txt ./ep1 $f > /dev/null
		/usr/bin/time -f "\n\nep1-mili\n%es\n%MKb" -ao relatorio.txt ./ep1-mili $f > /dev/null
		/usr/bin/time -f "\n\npep1\n%es\n%MKb" -ao relatorio.txt ./pep1 $f > /dev/null
		/usr/bin/time -f "\n\npep1-mili\n%es\n%MKb" -ao relatorio.txt ./pep1-mili $f > /dev/null
	fi
done

clear
PROCESSADOR=`cat /proc/cpuinfo|grep "vendor_id" |cut -d ':' -f 2 | tr -d [:blank:] | head -1`
MODELOPROC=`cat /proc/cpuinfo|grep "model name" |cut -d ':' -f 2 | tr -d [:blank:] | head -1`
CORES=`cat /proc/cpuinfo|grep "cpu cores" |cut -d ':' -f 2 | tr -d [:blank:] | head -1`
CLOCK=`cat /proc/cpuinfo|grep "cpu MHz" |cut -d ':' -f 2 | tr -d [:blank:] | head -1`
CACHE=`cat /proc/cpuinfo|grep "cache size" |cut -d ':' -f 2 | tr -d [:blank:] | head -1`
FILENAME="$HOSTNAME.$PROCESSADOR.tex"

echo -en "\t$HOSTNAME & $PROCESSADOR & $MODELOPROC & $CORES & $CLOCK & $CACHE \\" > $FILENAME
echo -e "\\ \n\t\\hline" >> $FILENAME

echo $FILENAME

