#include <stdio.h>
#include <stdlib.h>

int main(){
    char nome[100];
    printf("Nome do arquivo: ");
    scanf("%s",nome);
    FILE *arq = fopen(nome, "rb");

    printf("0 para int, 1 para string: \n");
    int tipo;
    scanf("%d",&tipo);

    char status;
    int qtdReg;

    fread(&status, sizeof(char), 1, arq);
    fread(&qtdReg, sizeof(int), 1, arq);

    printf("status:%c|qtdReg%d\n", status, qtdReg);

    if(tipo == 0){
        for(int i = 0; i < qtdReg; ++i){
            int campoIndexado;
            long int byte;
            fread(&campoIndexado, sizeof(int), 1, arq);
            fread(&byte, sizeof(long int), 1, arq);

            printf("campoIndexado:%d|byteOffSet:%ld\n", campoIndexado, byte);
        }
    }else{
        for(int i = 0; i < qtdReg; ++i){
            char string[13];
            long int byte;
            fread(string, sizeof(char), 12, arq);
            fread(&byte, sizeof(long int), 1, arq);
            string[12] = '\0';

            printf("campoIndexado:%s|byteOffSet:%ld\n", string, byte);
        }
    }
   

    return 0;
}