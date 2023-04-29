all: manipulacao.o arqIndice.o auxiliares.o main.o registros.o 
	gcc manipulacao.o arqIndice.o auxiliares.o main.o registros.o -o arq -Wall -std=c99;

manipulacao.o:
	gcc -c manipulacao.c -o manipulacao.o

arqIndice.o:
	gcc -c arqIndice.c -o arqIndice.o

auxiliares.o:
	gcc -c auxiliares.c -o auxiliares.o

registros.o:
	gcc -c registros.c -o registros.o

main.o:
	gcc -c main.c -o main.o

run:
	./arq

clean:
	rm *.o arq

