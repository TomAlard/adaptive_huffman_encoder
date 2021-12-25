#!/bin/bash


FILE="canterbury_corpus/bible.txt"
for LEN in {1..9}
do
	for MEM in {0..9}
	do
		cat "$FILE" | ./a.out -c $LEN,$MEM > temp
		TIME=$((time cat temp | ./a.out -d) 2>&1 > /dev/null | grep real | awk '{print $2}' | sed 's/.*m//' | sed 's/s//')
		echo "LEN: $LEN, MEM: $MEM, TIME (seconds): $TIME"
	done
done
