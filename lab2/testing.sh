#!/bin/bash
out=""
while read -r inline
do
	gcc zad23.c -o zad23 && ./zad23 < $inline > $out 
	if [[ $out -eq $(read -r outline < "outs.txt") ]] ; then
		echo "OK"
	else
		echo $inline
		echo $out
		echo $outline
	fi	
done < tests.txt
