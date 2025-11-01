all: main

main: src/main.c src/util.c
	gcc -Iinclude -o dist/main src/main.c src/util.c

clean:
	rm -f main