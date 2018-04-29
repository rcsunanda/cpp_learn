#!/bin/bash

echo "------------------ 1"	##########################

1>test.txt	#does not work, just creates an empty file

echo "------------------ 2"	##########################

{
	echo "AAAA"
	echo "BBBB"
} > test.txt	#redirection applies to all commands in block


echo "------------------ 3"	##########################

{
ls -lh

ls -lh XXXX	#should output error on stderr
} > test.txt

echo "------------------ 4"	##########################

#Redirecting part of a script with exec

exec 3>&1 4>&2 >test.txt 2>&1     #save stdout in 3 and stderr in 4, redirect stdout and stderr to file.txt

#part of script with redirections to file.txt
echo "Normal Msg"
echo "error msg" >&2

exec 1<&- 2<&-     #close FDs
exec 1>&3 2>&4     #restore stdout and stderr

#rest of script (no redirections)


echo "------------------ 5"	##########################

# i=0
# while sleep 3; do
	# (( ++i ))
	# echo "line # $i"
# done > lines.txt


echo "------------------ 6"	##########################

x=5

echo "Before, x=$x"

{ ls; x=33; } | { cat; x=44; }

echo "After, x=$x"
