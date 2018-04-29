#!/bin/bash

echo "------------------ 1"	##########################

names1=("Bob" "Peter" "$USER" "Big Bad John")

i=0
for element in "${names1[@]}"
do
	(( ++i ))
    echo "Iter no: $i, element: $element"
done

echo "Array indices: ${!names1[@]}"

echo "------------------ 2"	##########################

names2=([0]="Bob" [1]="Peter" [20]="$USER" [21]="Big Bad John" "G Man")
i=0
for element in "${names2[@]}"
do
	(( ++i ))
    echo "Iter no: $i, element: $element"
done

echo "Array indices: ${!names2[@]}"

echo "------------------ 3"	##########################

names3[0]="Bob"
names3[1]="Peter"
names3[20]="$USER"
names3[21]="Big Bad John"

i=0
for element in "${names3[@]}"
do
	(( ++i ))
    echo "Iter no: $i, element: $element"
done

echo "Array indices: ${!names3[@]}"

echo "------------------ 4"	##########################

declare -a names4

names4[0]="Bob"
names4[1]="Peter"

i=0
for element in "${names4[@]}"
do
	(( ++i ))
    echo "Iter no: $i, element: $element"
done

echo "Array indices: ${!names4[@]}"

echo "------------------ 5"	##########################

declare -a names5=("Bob" "Peter" "$USER" "Big Bad John")
i=0
for element in "${names5[@]}"
do
	(( ++i ))
    echo "Iter no: $i, element: $element"
done

echo "Array indices: ${!names5[@]}"

echo "------------------ 6"	##########################

names6=()

names6+=("Bob")
names6+=("Peter" "Holmes")

i=0
for element in "${names6[@]}"
do
	(( ++i ))
    echo "Iter no: $i, element: $element"
done

echo "Array indices: ${!names6[@]}"

echo "------------------ 7"	##########################

info="DOB DD-MM-YY"
declare -A assArray=(["Full Name"]="John Cena" ["City"]="Wattala" ["Occupation"]="Baas" [$info]="22/02/1922")

echo "Key: Full Name, value: ${assArray[Full Name]}"

#value iteration
i=0
for val in "${assArray[@]}"
do
	(( ++i ))
    echo "Iter no: $i, key: $key, element: $val"
done

#key-value iteration
for key in "${!assArray[@]}"
do
	echo "Key: $key, value: ${assArray[$key]}"
done
