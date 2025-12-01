chmod a+x scriptProy.sh

CC=gcc
SRC="pfsqat.c"
EXE=./pf.exe
INPUT=entrada7.txt
MODO="S"

$CC -O3 $SRC -o "$EXE" -lm  2>&1

OUT="outputPFSQAT.txt"
touch "$OUT"

for n in 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20; do
  echo "tamaño: $n" >> "$OUT"
  sudo perf stat -e power/energy-pkg/,power_core/energy-core/ "$EXE" $n "$MODO" < "$INPUT" >> "$OUT" 2>&1
done