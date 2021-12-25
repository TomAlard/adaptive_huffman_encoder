#!/bin/bash


mkdir 'canterbury_corpus_results'
FILES="canterbury_corpus/*"
for f in $FILES 
do
	base=$(basename $f)
	cat "$f" | ./a.out -c 9,9 > "canterbury_corpus_results/${base%%.*}.bin"
done
