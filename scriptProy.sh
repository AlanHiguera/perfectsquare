
CC=gcc
SRC="pfsqat.c"
BIN=./pf.exe
INPUT=entrada7.txt
MODO="S"

# Compilar una vez si el binario no existe
if [ ! -x "$BIN" ]; then
  echo "Compilando $SRC -> $BIN"
  $CC -O3 $SRC -o "$BIN" -lm -lgmp
fi

OUT="outputPFSQAT.txt"
touch "$OUT"

for n in 1 2 3 4 5 6 7 8 9 10 11 12; do
  echo "tamaño: $n" >> "$OUT"
  $BIN $n "$MODO" < "$INPUT" >> "$OUT"
done