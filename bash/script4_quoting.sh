#!/bin/bash

foo="aaa*"
path="aaa*"
 
if [[ $path = "$foo" ]]; then
   echo "true"
else
   echo "false"
fi

echo "########################################## 2"

#quoting in here documents

echo "current shell is: $$"

var="AA BBB"
arr=(words in array "two words together")
./passed_args_env ${arr[@]}

cat <<- EOF
	Array: ${arr[@]}
	Variable: $var
	Array with quotes: "${arr[@]}"
	Variale with quotes: "$var"
	*
EOF

echo "########################################## 3"

ssh -T localhost <<- EOF
	cd /mnt/hgfs/Ubuntu_VM_Shared_Folder/learn_projects/bash
	echo "label is not quoted"
	echo "$var"
	echo $var
	./passed_args_env "${arr[@]}"
	echo "----------------------- 31"
	./passed_args_env ${arr[@]}
	echo "----------------------- 32"
	arr2=(arr2el1 arr2el2 "arr2 words together")
	./passed_args_env "${arr2[@]}"
EOF

echo "########################################## 4"

ssh -T localhost <<- "EOF"
	cd /mnt/hgfs/Ubuntu_VM_Shared_Folder/learn_projects/bash
	echo "label is quoted"
	var="Remote CC DD"
	echo "$var"
	echo $var
	./passed_args_env "${arr[@]}"
	echo "----------------------- 41"
	./passed_args_env ${arr[@]}
	echo "----------------------- 42"
	arr2=(arr2el1 arr2el2 "arr2 words together")
	./passed_args_env "${arr2[@]}"
EOF

# bash <<- EOF
	# echo "current shell is: $$"
	# #./passed_args_env "${arr[@]}"
	# sleep 5
# EOF