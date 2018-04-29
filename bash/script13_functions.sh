#!/bin/bash

function display ()
{
	echo "No. of args: $#"
	echo "$@"
	var="XXX"
	var2="YYYY"
}

echo "Func definition return code: $?"

echo "------------------ 1"	##########################

trap '(read -p "[$BASH_SOURCE:$LINENO] $BASH_COMMAND?")' DEBUG

var="AA BB"
display "$var" "some de" "cristo"

echo "Func call return code: $?"

echo "var now: $var"
echo "var2 now: $var2"

echo "------------------ 2"	##########################


echo "------------------ 3"	##########################


echo "------------------ 5"	##########################

echo "------------------ 6"	##########################
