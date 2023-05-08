#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "arq_indice.h"
#include "auxiliares.h"

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

cabecalho_indx_t *ler_index_cabecalho(FILE *arq){
	//Lê e retorna um ponteiro para o cabeçalho do arquivo 
	erro(arq);

	cabecalho_indx_t *cabecalho_retorno = alocar_cbl_indx();

    fread(&(cabecalho_retorno->status),sizeof(char),1,arq);
    fread(&(cabecalho_retorno->qtdReg),sizeof(int),1,arq);
	
	return cabecalho_retorno;
}

dados_indx_int_t **ler_index_dado_int(FILE *arq, cabecalho_indx_t *cabecalho){
    dados_indx_int_t **vetor_dados = aloc_vet_indx_DadoInt(cabecalho->qtdReg);

    for(int i=0; i<cabecalho->qtdReg; i++){
        fread(&(vetor_dados[i]->chaveBusca),sizeof(int),1,arq);
        fread(&(vetor_dados[i]->byteOffset),sizeof(long int),1,arq);
    }

    return vetor_dados;
}


dados_indx_str_t **ler_index_dado_str(FILE *arq, cabecalho_indx_t *cabecalho){
    dados_indx_str_t **vetor_dados = aloc_vet_indx_DadoStr(cabecalho->qtdReg);

    for(int i=0; i<cabecalho->qtdReg; i++){
        fread((vetor_dados[i]->chaveBusca),sizeof(char),TAM_CAMP_STR,arq);
        fread(&(vetor_dados[i]->byteOffset),sizeof(long int),1,arq);
    }

    return vetor_dados;
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

void mostraVetInt(void *vet_generico, int qntd_reg){
    dados_indx_int_t **vet = (dados_indx_int_t**)vet_generico;
    for(int i = 0; i < qntd_reg; ++i){
        mostraRegIndx_int(vet[i]);
    }
}

void mostraVetStr(void *vet_generico, int qntd_reg){
    dados_indx_str_t **vet = (dados_indx_str_t**)vet_generico;
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

void ordenaVetIndex_int(void *vetor_generico, int qntd_reg){
    dados_indx_int_t **vetor_real = (dados_indx_int_t**)vetor_generico;
    qsort(vetor_real, qntd_reg, sizeof(dados_indx_int_t*), compara_int);
}

void ordenaVetIndex_str(void *vetor_generico, int qntd_reg){
    dados_indx_str_t **vetor_real = (dados_indx_str_t**)vetor_generico;
    qsort(vetor_real, qntd_reg, sizeof(dados_indx_str_t*), compara_str);
}

void escreveCabecalhoIndex(FILE *arqIndex, cabecalho_indx_t *cabecalho){
    fwrite(&cabecalho->status,sizeof(char),1,arqIndex);
}

int busca_bin_rec_int(dados_indx_int_t **vetor, int ini, int fim, int chave){
    printf("ini=%d,fim=%d\n",ini,fim);

    if(ini > fim){
        printf("vo retornar -1\n");
        return -1; //não foi encontrado    
    } 
    

    int meio = (ini+fim)/2;
    printf("meio = %d\n",meio);

    printf("chave=%d\n",chave);

    printf("vetor[%d]=%d\n",meio,vetor[meio]->chaveBusca);

    if((vetor[meio]->chaveBusca)==chave){
        printf("achei %d na posicao %d\n",chave,meio);
        return meio;
    }
    else if((vetor[meio]->chaveBusca)>chave){
        fim = meio-1;
        printf("fim agr eh o meio\n");
        return busca_bin_rec_int(vetor, ini, fim, chave);
    }else{ //((vetor[meio]->chaveBusca)<chave)
        ini = meio+1;
        printf("ini agr eh o meio\n");
        return busca_bin_rec_int(vetor, ini, fim, chave);
    }
}

int busca_bin_int(dados_indx_int_t **vetor, cabecalho_indx_t *cabecalho,int chave){
    printf("busca bin int vai chamar o recursivo\n");
    int pos = busca_bin_rec_int(vetor,0,cabecalho->qtdReg,chave);
    printf("fiz a recursao, deu pos = %d\n",pos);
    
    if(pos != -1){
        /*Como há campos que podem ter valores iguais, a busca binaria retorna a posição de um dos valores que satisfaz a busca.
        No arquivo de index, esses valores estão ordenados, então quero o primeiro deles*/
        while(((pos-1)>=0) && ((vetor[pos-1]->chaveBusca) == (vetor[pos]->chaveBusca))){
            //o teste ((pos-1) >=0) deve ser feito para evitar segmentation fault caso 'pos' seja igual a zero
            pos--; //posição do primeiro registro que satisfaz a busca no vetor
        }
    }
    return pos; 
}


int busca_bin_rec_str(dados_indx_str_t **vetor, int ini, int fim, char *chave){
    printf("ini=%d,fim=%d\n",ini,fim);
    if(ini > fim){
        printf("vo retornar -1\n");
        return -1;
    } 

    int meio = (ini+fim)/2;
    printf("meio = %d\n",meio);

    printf("chave=%s\n",chave);

    printf("vetor[%d]=%s\n",meio,vetor[meio]->chaveBusca);

    if(strcmp(vetor[meio]->chaveBusca,chave)==0){
        printf("achei %s na posicao %d\n",chave,meio);
        return meio;
    }else if(strcmp(vetor[meio]->chaveBusca,chave)>0){//(vetor[meio]->chaveBusca) > chave
        fim = meio;
        printf("fim agr eh o meio\n");
        return busca_bin_rec_str(vetor, ini, fim, chave);
    }else{//(vetor[meio]->chaveBusca) < chave
        ini = meio;
        printf("ini agr eh o meio\n");
        return busca_bin_rec_str(vetor, ini, fim, chave);
    }
}

int busca_bin_str(dados_indx_str_t **vetor, cabecalho_indx_t *cabecalho, char *chave){
    printf("busca bin int vai chamar o recursivo\n");
    int pos = busca_bin_rec_str(vetor,0,cabecalho->qtdReg,chave);
    printf("fiz a recursao, deu pos = %d\n",pos);
    if(pos != -1){
        /*Como há campos que podem ter valores iguais, a busca binaria retorna a posição de um dos valores que satisfaz a busca.
        No arquivo de index, esses valores estão ordenados, então quero o primeiro deles*/
        while(((pos-1)>=0) && strcmp(vetor[pos-1]->chaveBusca, vetor[pos]->chaveBusca)==0){
            //o teste ((pos-1) >=0) deve ser feito para evitar segmentation fault caso 'pos' seja igual a zero
            pos--; 
        }    
    }

    return pos; //posição do primeiro registro que satisfaz a busca no vetor ou -1 caso nenhum satisfaça
}