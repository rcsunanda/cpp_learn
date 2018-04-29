#!/bin/bash

if ls aaaa.txt; then
	echo "AA"
	echo "BB"; echo "CC";
elif ls file.txt; then
	echo "XX"
	echo "YY"; echo "ZZ";
else
	echo "PP"
	echo "QQ"; echo "RR";
fi
	
##############

if [ aaa = bbbb ]; then
	echo "[ a = b ] returned true"
else
	echo "[ a = b ] returned false"
fi
	
##############

echo "Using [[ and ]] keywords"

str1="Sunanda Gamage"
str2="Sunanda Gamage"
[[ $str1 = $str2 ]]
echo "Return value: " $?	#returns true (0)

if [[ aaa = a* ]]; then
	echo "[ a = b ] returned true"
else
	echo "[ a = b ] returned false"
fi

filename=myfile.txt
pattern=*.txt

[[ $filename = $pattern ]] && echo "%%% returned true"    #without quotes, pattern matching happens; this returns true
echo "matched result: " $? 	#true

[[ $filename = "$pattern" ]]     #with quotes, literal string matching happens; this returns false
echo "matched result: " $?

###############

echo "###########################"

[[ 5 -lt 2 && ( 10 -lt 30 || aa < zz ) ]]    
echo "Return value: " $?

echo "###########################"

