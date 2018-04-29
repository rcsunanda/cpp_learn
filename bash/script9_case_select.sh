#!/bin/bash

echo "--------- 1"

case $LANG in
	en*) echo 'Hello!' ;;
	fr*) echo 'Salut!' ;;
	de*) echo 'Guten Tag!' ;;
	nl*) echo 'Hallo!' ;;
	it*) echo 'Ciao!' ;;
	es*) echo 'Hola!' ;;
	C|POSIX) echo 'hello world' ;;
	*) echo 'I do not speak your language.' ;;
esac

echo "--------- 2"

echo "Which of these does not belong in the group?"

select choice in Apples Pears Crisps Lemons Kiwis
do
	if [[ $choice = Crisps ]]; then 
		echo "Correct! Crisps are not fruit.";
		break;
	fi
	
	echo "Errr... no. Try again."
done

echo "--------- 3"

