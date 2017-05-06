all:
	@gcc src/main.c src/funcoes.c -o bin/prog
	@./bin/prog