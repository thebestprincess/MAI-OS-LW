all: libd1.so libd2.so prog1 prog2

libd1.so: lib1.c
	gcc -fPIC -shared lib1.c -o libd1.so -lm

libd2.so: lib2.c
	gcc -fPIC -shared lib2.c -o libd2.so -lm

prog1: prog1.c
	gcc prog1.c -L. -ld1 -o prog1 -Wl,-rpath,.

prog2: prog2.c
	gcc prog2.c -o prog2 -ldl

clean:
	rm -f *.so prog1 prog2