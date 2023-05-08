#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "arq_indice.h"

#define TAM_CAMP_STR 12

//Cabeçalho do arquivo de index
struct Cabecalho_indx{
    char status;
    int qtdReg;
};

//Registro de dados do aquivo index se o campo
//indexado for inteiro
struct Dados_indx_int{
    int chaveBusca;
    long int byteOffset;
};

//Registro de dados do aquivo index se o campo
//indexado for string
struct Dados_indx_str{
    char chaveBusca[TAM_CAMP_STR];
    long int byteOffset;
};

cabecalho_indx_t *alocar_cbl_indx(void){
    //Alocar o cabeçalho do arquivo de Índice
    cabecalho_indx_t *cbc_indx;//cabeçalho do Índex
    cbc_indx = malloc(sizeof(cabecalho_indx_t));
    return cbc_indx;
}

dados_indx_int_t *alocDadoIndxInt(void){
    dados_indx_int_t *indxDado = malloc(sizeof(dados_indx_int_t));
    return indxDado; 
}

dados_indx_str_t *alocDadoIndxStr(void){
    dados_indx_str_t *indxDado = malloc(sizeof(dados_indx_str_t));
    return indxDado; 
}

dados_indx_int_t **aloc_vet_indx_DadoInt(int nroRegValidos){
    dados_indx_int_t **vet_retorno;
    vet_retorno = (dados_indx_int_t**)malloc(sizeof(dados_indx_int_t*) * nroRegValidos);
    for(int i = 0; i < nroRegValidos; ++i){
        vet_retorno[i] = malloc(sizeof(dados_indx_int_t));
    }
    return vet_retorno;
}

dados_indx_str_t **aloc_vet_indx_DadoStr(int nroRegValidos){
    dados_indx_str_t **vet_retorno;
    vet_retorno = (dados_indx_str_t**)malloc(sizeof(dados_indx_str_t*) * nroRegValidos);
    for(int i = 0; i < nroRegValidos; ++i){
        vet_retorno[i] = malloc(sizeof(dados_indx_str_t));
    }
    return vet_retorno;
}

char *alocarCampoIndexado(void){
    char *vet_retorno = malloc(sizeof(char)*TAM_CAMP_STR);
    return vet_retorno;
}

void setCabecalhoIndex(cabecalho_indx_t *cabecalho, const char status){
    cabecalho->status = status;
}

void setDadoIndxInt(dados_indx_int_t *dado, long int byteOffSet, int valor){
    dado->byteOffset = byteOffSet;
    dado->chaveBusca = valor;
}


void setDadoIndxStr(dados_indx_str_t *dado, long int byteOffSet, char *valor){
    dado->byteOffset = byteOffSet;
    strcpy(dado->chaveBusca, valor);
}

void copiaDadoIndex_int(dados_indx_int_t *destino, dados_indx_int_t *origem){
    destino->byteOffset = origem->byteOffset;
    destino->chaveBusca = origem->chaveBusca;
}

void copiaDadoIndex_str(dados_indx_str_t *destino, dados_indx_str_t *origem){
    destino->byteOffset = origem->byteOffset;
    strcpy(destino->chaveBusca, origem->chaveBusca);
}

void mostraRegIndx_int(dados_indx_int_t *dado){
    printf("campoIndexado:%d|byte:%ld\n", dado->chaveBusca, dado->byteOffset);
}

void mostraRegIndx_str(dados_indx_str_t *dado){
    printf("campoIndexado:%s|byte:%ld\n", dado->chaveBusca, dado->byteOffset);
}

void mostraVetInt(dados_indx_int_t **vet, int qntd_reg){
    for(int i = 0; i < qntd_reg; ++i){
        mostraRegIndx_int(vet[i]);
    }
}

void mostraVetStr(dados_indx_str_t **vet, int qntd_reg){
    for(int i = 0; i < qntd_reg; ++i){
        mostraRegIndx_str(vet[i]);
    }
}

int compara_int(const void *a, const void *b){
    int valor_a = (*((dados_indx_int_t**)a))->chaveBusca;
    int valor_b = (*((dados_indx_int_t**)b))->chaveBusca;
    return (valor_a - valor_b);
}

int compara_str(const void *a, const void *b){
    char str_a[TAM_CAMP_STR];
    char str_b[TAM_CAMP_STR];
    strcpy(str_a, (*((dados_indx_str_t**)a))->chaveBusca);
    strcpy(str_b, (*((dados_indx_str_t**)b))->chaveBusca);
    return strcmp(str_a, str_b);
}

void ordenaVetIndex_int(dados_indx_int_t **vet, int qntd_reg){
    qsort(vet, qntd_reg, sizeof(dados_indx_int_t*), compara_int);
}

void ordenaVetIndex_str(dados_indx_str_t **vet, int qntd_reg){
    qsort(vet, qntd_reg, sizeof(dados_indx_str_t*), compara_str);
}

void escreveCabecalhoIndex(FILE *arqIndex, cabecalho_indx_t *cabecalho){
    fwrite(&cabecalho->status,sizeof(char),1,arqIndex);
}

void busca_bin_int(cabecalho_indx_t *cabecalho){

}