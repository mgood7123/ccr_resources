echo pre-compiling leg program
leg -P asm.leg -o asm.h
echo compiling program
gcc example.c -o example -lpcre2-8 -lm -w
echo executing program
./example
