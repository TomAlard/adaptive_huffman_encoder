#!/bin/bash


mkdir canterbury_corpus
cd canterbury_corpus
wget -O corpus.tar.gz 'corpus.canterbury.ac.nz/resources/cantrbry.tar.gz'
tar -xvzf corpus.tar.gz
rm -f corpus.tar.gz alice29.txt asyoulik.txt cp.html fields.c grammar.lsp sum xargs.1

wget -O artificial_corpus.tar.gz 'corpus.canterbury.ac.nz/resources/artificl.tar.gz'
tar -xvzf artificial_corpus.tar.gz
rm -f artificial_corpus.tar.gz a.txt aaa.txt

wget -O large_corpus.tar.gz 'corpus.canterbury.ac.nz/resources/large.tar.gz'
tar -xvzf large_corpus.tar.gz
rm -f large_corpus.tar.gz E.coli world192.txt

wget -O gutenberg_psyche.txt 'https://www.gutenberg.org/cache/epub/66555/pg66555.txt'
