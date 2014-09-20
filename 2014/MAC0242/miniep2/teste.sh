#!/bin/bash

for f in `ls teste/`; do
    OUTA=`./miniep2.py -a < teste/$f`
    OUTB=`./miniep2.py -b < teste/$f`
    GABA=`cat gabarito/a$f`
    GABB=`cat gabarito/b$f`
    

    if [ "$OUTA" == "$GABA" ]
    then
    	echo "O teste a$f passou com sucesso!"
	else
		echo "O teste a$f falhou! =("
	fi

	if [ "$OUTB" == "$GABB" ]
    then
    	echo "O teste b$f passou com sucesso!"
	else
		echo "O teste b$f falhou! =("
	fi
done