#!/bin/bash -  

# Criando todos os binarios para gerar um relatorio com estatística de tempo.
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