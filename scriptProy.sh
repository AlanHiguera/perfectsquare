#!/usr/bin/env bash
# run_grid.sh - ejecutar pf.exe para varios n

CC=gcc
# Ajusta los fuentes según tu proyecto
SRC="pfsqat.c"
# BINARY que se ejecutará
BIN=./pf.exe
INPUT=entrada7.txt


for n in 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15; do
  echo "=== n=$n ==="
  OUT="outputPFSQAT$.txt"
  echo "tamaño: $n" >> "$OUT"
  $BIN $n < "$INPUT" >> "$OUT"
done