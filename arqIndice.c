#include <stdio.h>
#include <stdlib.h>
#include "arqIndice.h"

//Cabeçalho do arquivo de index
struct Cabecalho_indx{
    char status;
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
    char chaveBusca[12];
    long int byteOffset;
};

cabecalho_indx_t *alocar_cbl_indx(void){
    //Alocar o cabeçalho do arquivo de Índice
    cabecalho_indx_t *cbc_indx;//cabeçalho do Índex
    cbc_indx = malloc(sizeof(cabecalho_indx_t));
    return cbc_indx;
}

dados_indx_int_t *aloc_vet_DadoInt(int nroRegValidos){
    dados_indx_int_t *vet_retorno;
    vet_retorno = (dados_indx_int_t*)malloc(sizeof(dados_indx_int_t) * nroRegValidos);
    return vet_retorno;
}

dados_indx_str_t *aloc_vet_DadoStr(int nroRegValidos){
    dados_indx_str_t *vet_retorno;
    vet_retorno = (dados_indx_str_t*)malloc(sizeof(dados_indx_str_t) * nroRegValidos);
    return vet_retorno;
}