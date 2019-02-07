echo pre-compiling leg program
leg -P ISA.leg -o ISA.c
echo compiling program
gcc ISA.c -o ISA -lpcre2-8 -lm -w
echo executing program
./ISA ./Intel4004.isa
