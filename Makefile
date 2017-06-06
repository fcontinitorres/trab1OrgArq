all: compile run clean

compile:
	gcc src/main.c src/funcoes.c -o bin/prog -g

run:
	./bin/prog

debug:
	valgrind -v --leak-check=full --show-leak-kinds=all --read-var-info=yes --track-origins=yes ./bin/prog

clean:
	rm -f bin/prog
	rm -f vgcore*
	rm -r *.o

zip:
	zip -r parte2.zip ./
