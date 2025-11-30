#!/usr/bin/env bash
# Ensure script is executable for everyone
chmod a+x scriptProy.sh

# Solicitar contraseña de sudo una sola vez al inicio

CC=gcc
SRC="pfsqat.c"
BIN=./pf.exe
INPUT=entrada7.txt
MODO="S"

# Compilar una vez si el binario no existe
if [ ! -x "$BIN" ]; then
  echo "Compilando $SRC -> $BIN"
  $CC -O3 $SRC -o "$BIN" -lm -lgmp 2>&1 | grep -i error || true
fi

OUT="outputPFSQAT.txt"
touch "$OUT"

for n in 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15; do
  echo "tamaño: $n" >> "$OUT"
  sudo perf stat -e power/energy-pkg/,power_core/energy-core/ "$BIN" $n "$MODO" < "$INPUT" >> "$OUT" 2>&1
done