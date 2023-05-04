#include <stdio.h>
#include <stdlib.h>
#include "arq_dados.h"
#include "auxiliares.h"

#define TAM_MAX_CABECALHO_CSV 100

void binarioNaTela(char *nomeArquivoBinario){
    /*
     * Você não precisa entender o código dessa função.
     *
     * Use essa função para comparação no run.codes.
     * Lembre-se de ter fechado (fclose) o arquivo anteriormente.
     * Ela vai abrir de novo para leitura e depois fechar
     * (você não vai perder pontos por isso se usar ela).
     */

    unsigned long i, cs;
    unsigned char *mb;
    size_t fl;
    FILE *fs;
    if (nomeArquivoBinario == NULL || !(fs = fopen(nomeArquivoBinario, "rb"))) {
        fprintf(stderr,
                "ERRO AO ESCREVER O BINARIO NA TELA (função binarioNaTela): "
                "não foi possível abrir o arquivo que me passou para leitura. "
                "Ele existe e você tá passando o nome certo? Você lembrou de "
                "fechar ele com fclose depois de usar?\n");
        return;
    }
    fseek(fs, 0, SEEK_END);
    fl = ftell(fs);
    fseek(fs, 0, SEEK_SET);
    mb = (unsigned char *)malloc(fl);
    fread(mb, 1, fl, fs);

    cs = 0;
    for (i = 0; i < fl; i++) {
        cs += (unsigned long)mb[i];
    }
    printf("%lf\n", (cs / (double)100));
    free(mb);
    fclose(fs);
}

int tamanhoStr(char *string){
    int tamanho = 0;
    char cursor;
    do{
        cursor = string[tamanho];
        printf("cursor:%c|tam:%d\n", cursor, tamanho);
        tamanho++;
    }while(cursor != '|');
    return tamanho;
}

char *alocar_nome(int tamanho_nome){
    char *nome = malloc(sizeof(char) * tamanho_nome);
    return nome;
}

void erro(void *ponteiro){
    //verifica a existência de algum erro ao longo das funcionalidades
    if(ponteiro == NULL){
        printf("Falha no processamento do arquivo.\n");
        exit(0);
    }
}

void pular_campos_fixos(FILE *arq_bin){
    //pula os campos fixos exceto o campo 'removido'
    int qntd_bytes_pular = 2*sizeof(int) + 23*sizeof(char);
    fseek(arq_bin, qntd_bytes_pular, SEEK_CUR);
}

void pular_ate_fim_registro(FILE *arq_bin){
    //função que pula até o fim de um registro
    char cursor;
    do{
        fread(&cursor, sizeof(char), 1, arq_bin);
    }while(cursor != '#');
}

void pular_registro_removido(FILE *arq_bin){
    pular_campos_fixos(arq_bin);
    pular_ate_fim_registro(arq_bin);
}

void pular_cabecalho_csv(FILE *arq){
    //função que lê e ignora a primeira linha do arquivo csv

    char texto[TAM_MAX_CABECALHO_CSV];

    //lê-se, caractere a caractere, a linha até encontrar a quebra de linha
    int i=-1;
    do{
        i++;
        fscanf(arq,"%c",&texto[i]);
    }while(texto[i] != '\n');
}

void mensagem_NULO(void){
    printf("NULO, ");
}