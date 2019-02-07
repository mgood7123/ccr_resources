echo compiling program &&
gcc parser.c -o parser -w -g3 &&
echo executing program &&
./parser &&
echo compiling produced program &&
gcc ./tmpfile.c -o ./tmpfile -w -g3 &&
echo executing produced program &&
./tmpfile
