#!/bin/bash

filename=$1
x=0
arr=()
while [ $x -le 127 ]
do
	echo "$filename"
	file $filename | grep "zip"
	if [ "$?" -eq "0" ]
	then
		echo "Unzipping $filename"
		
		if unzip -P "0" $filename ; then
		    arr+=(0)
		else
		    unzip -P "1" $filename
		    arr+=(1)
		fi

		rm $filename
		filename=$(ls *zip)
		x=$(( $x + 1 ))
	fi
done
echo ${arr[@]}