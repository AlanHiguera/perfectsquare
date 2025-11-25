#!/usr/bin/env bash
# run_grid.sh - ejecutar pf.exe para varios n

CC=gcc
# Ajusta los fuentes según tu proyecto
SRC="pfsqat.c"
# BINARY que se ejecutará
BIN=./pf.exe
INPUT=entrada7.txt

# Compilar antes de ejecutar (si ya está compilado puedes comentar esta línea)
echo "Compilando $SRC -> $BIN"
$CC -O3 -Wall $SRC -o "$BIN" -lm -lgmp || { echo "Compilación fallida"; exit 1; }

# Si quieres perfilar con gprof, compila con -pg en su lugar:
# $CC -pg -g -O2 $SRC -o "$BIN" -lm -lgmp

for n in 1 2 3 4 5 6 7 8 9 10 11; do
  echo "=== n=$n ==="
  OUT="outputPFSQAT$.txt"
  echo "tamaño: $n" >> "$OUT"
  $BIN $n < "$INPUT" >> "$OUT"
done