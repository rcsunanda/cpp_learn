#!/bin/bash

echo "--------- 1"

i=1
while (( i <= 10 ))
do
	echo "$i empty cans of beer."
	(( ++i ))
done

echo "--------- 2"

for (( i=1; i <= 10; i++ ))
do
	echo "$i empty cans of beer."
done

echo "--------- 3"

for i in {1..10}
do
	echo "$i empty cans of beer."
done