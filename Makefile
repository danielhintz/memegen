all: memegen

install: memegen
	cp memegen /usr/bin/memegen

memegen: memegen.o
	gcc -o memegen memegen.o -lpng

memegen.o: memegen.c
	gcc -c memegen.c

clean:
	rm -f memegen.o memegen
