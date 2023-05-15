#include <stdio.h>
#include <stdlib.h>

int main(){
    char nome[] = "indice6.bin";
    FILE *arq = fopen(nome, "rb");

    char status;
    int qtdReg;

    fread(&status, sizeof(char), 1, arq);
    fread(&qtdReg, sizeof(int), 1, arq);

    printf("status:%c|qtdReg%d\n", status, qtdReg);

    for(int i = 0; i < qtdReg+10; ++i){
        int campoIndexado;
        long int byte;
        fread(&campoIndexado, sizeof(int), 1, arq);
        fread(&byte, sizeof(long int), 1, arq);

        printf("campoIndexado:%d|byteOffSet:%ld\n", campoIndexado, byte);
    }

    return 0;
}