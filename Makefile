all: memegen

memegen: memegen.o
	gcc -o memegen memegen.o -lpng

memegen.o: memegen.c
	gcc -c memegen.c

clean:
	rm memegen.o memegen
