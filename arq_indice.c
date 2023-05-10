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

int comparacao_vet_dados_indx_int(void *ponteiro, int pos1, int pos2){
    //funcao que dado um vetor de dados_indx_int_t, compara duas posições

    dados_indx_int_t **vetor_casting = (dados_indx_int_t **) ponteiro;
    if(vetor_casting[pos1]->chaveBusca != vetor_casting[pos2]->chaveBusca){
        //se os inteiros forem diferentes, retorno 0
        return 0;
    }
    //se forem iguais, retorno 1
    return 1;
}

int comparacao_vet_dados_indx_str(void *ponteiro, int pos1, int pos2){
    //funcao que dado um vetor de dados_indx_str_t, compara duas posições

    dados_indx_str_t **vetor_casting = (dados_indx_str_t **) ponteiro;

    if(comparar_n_chars(vetor_casting[pos1]->chaveBusca,vetor_casting[pos2]->chaveBusca,TAM_CAMP_STR)!=0){
        //se as strings nao forem iguais, retorno 0
        return 0;
    }
    //se forem iguais, retorno 1
    return 1;
}

int tratamento(int pos, int *qtd_reg_val, void *vetor, int (*comparacao)(void*,int,int)){
    //funcao que trata o retorno da busca binaria recursiva

    if(pos == -1){
        //se nenhum registro satisfaz o critério, nao preciso fazer os tratamentos que ocorrem a seguir
        return pos; 
    }

    /*Como há campos que podem ter valores iguais, a busca binaria retorna a posição de um dos valores que satisfaz a busca.
    No arquivo de index, esses valores estão ordenados, então quero o primeiro deles*/
    while(((pos-1)>=0) && comparacao(vetor,pos,pos-1)){
        //o teste ((pos-1)>=0) deve ser feito para evitar segmentation fault caso 'pos' seja igual a zero
        pos--; //posição do primeiro registro que satisfaz a busca no vetor
    }
    
    //A partir do primeiro, conto quantos registros tem a mesma chave de busca e salvo essa info no qtd_reg_val
    int pos_aux = pos;
    do{
        (*qtd_reg_val)++;
        pos_aux++;
    }while(comparacao(vetor,pos_aux,pos_aux-1));
    //enquanto a chave de busca do atual for igual a do que acabou de ser contado

    return pos; 
}

int comparacao_vet_dados_indx_str_const(void *vetor, int pos, void *ponteiro){
    //funcao que dado um vetor de dados_indx_str_t, compara uma posição com uma string 
    dados_indx_str_t **vetor_casting = (dados_indx_str_t **) vetor;
    char *string = (char *)ponteiro;

    int comparacao = comparar_n_chars(vetor_casting[pos]->chaveBusca,string,TAM_CAMP_STR);

    if(comparacao==0){
        //se as strings forem iguais, retorno 0
        return 0;
    }else if(comparacao>0){
        //se o vetor_casting[pos] for maior que a string, retorno 1
        return 1;
    }else{
        //se o vetor_casting[pos] for menor que a string, retorno -1
        return -1;
    }
}

int comparacao_vet_dados_indx_int_const(void *vetor, int pos, void *ponteiro){
    //funcao que dado um vetor de dados_indx_int_t, compara uma posição com um inteiro
    dados_indx_int_t **vetor_casting = (dados_indx_int_t **) vetor;
    int *inteiro = (int *)ponteiro;

    if(vetor_casting[pos]->chaveBusca == (*inteiro)){
        //se os inteiros forem iguais, retorno 0
        return 0;
    }else if(vetor_casting[pos]->chaveBusca > (*inteiro)){
        //se o vetor_casting[pos] for maior que o inteiro, retorno 1
        return 1;
    }else{
        //se o vetor_casting[pos] for menor que o inteiro, retorno -1
        return -1;
    }
}

int busca_bin_rec(void *vetor, int ini, int fim, void *chave, int(*comparacao)(void*,int,void*)){
    //busca binaria recursiva
    if(ini > fim){//criterio de parada
        return -1;
    } 

    int meio = (ini+fim)/2;

    if(comparacao(vetor,meio,chave)==0){
        //se o vetor[meio] == chave, retorno o meio
        return meio;
    }else if(comparacao(vetor,meio,chave)==1){
        //se o vetor[meio] > chave, busco de novo até o meio-1
        fim = meio-1;

        return busca_bin_rec(vetor, ini, fim, chave, comparacao);
    }else{
        //se o vetor[meio] < chave, busco de novo a partir do meio+1
        ini = meio+1;

        return busca_bin_rec(vetor, ini, fim, chave, comparacao);
    }
}

int busca_bin_int(dados_indx_int_t **vetor, cabecalho_indx_t *cabecalho,int chave, int *qtd_reg_val){
    //funcao que prepara para a busca binaria recursiva para tipo inteiro e trata o retorno
    int pos = busca_bin_rec(vetor,0,cabecalho->qtdReg,&chave,comparacao_vet_dados_indx_int_const);
    return tratamento(pos,qtd_reg_val,vetor,comparacao_vet_dados_indx_int);
}

int busca_bin_str(dados_indx_str_t **vetor, cabecalho_indx_t *cabecalho, char *chave, int *qtd_reg_val){
    //funcao que prepara para a busca binaria recursiva para tipo string e trata o retorno
    int pos = busca_bin_rec(vetor,0,cabecalho->qtdReg,chave,comparacao_vet_dados_indx_str_const);
    return tratamento(pos,qtd_reg_val,vetor,comparacao_vet_dados_indx_str);
}

long int get_byteOffset_int(void *ponteiro, int pos){
    //funcao que retorna o byteoffset de um registro dentro de um vetor de registros de dados de um arquivo de indice tipo int
    dados_indx_int_t **registro = (dados_indx_int_t **)ponteiro;
    return registro[pos]->byteOffset;
}

long int get_byteOffset_str(void *ponteiro, int pos){
    //funcao que retorna o byteoffset de um registro dentro de um vetor de registros de dados de um arquivo de indice tipo string
    dados_indx_str_t **registro = (dados_indx_str_t **)ponteiro;
    return registro[pos]->byteOffset;
}





/*-----------------------------SO PRA GARANTIR---------------------------*/
int busca_bin_rec_int(dados_indx_int_t **vetor, int ini, int fim, int chave){
    if(ini > fim){
        return -1; //não foi encontrado    
    } 

    int meio = (ini+fim)/2;

    if((vetor[meio]->chaveBusca)==chave){
        return meio;
    }
    else if((vetor[meio]->chaveBusca)>chave){
        fim = meio-1;
        return busca_bin_rec_int(vetor, ini, fim, chave);
    }else{ //((vetor[meio]->chaveBusca)<chave)
        ini = meio+1;
        return busca_bin_rec_int(vetor, ini, fim, chave);
    }
}






int busca_bin_rec_str(dados_indx_str_t **vetor, int ini, int fim, char *chave){
    if(ini > fim){
        return -1;
    } 

    int meio = (ini+fim)/2;

    if(comparar_strings(vetor[meio]->chaveBusca,chave)==0){

        return meio;
    }else if(comparar_strings(vetor[meio]->chaveBusca,chave)>0){//(vetor[meio]->chaveBusca) > chave
        fim = meio-1;

        return busca_bin_rec_str(vetor, ini, fim, chave);
    }else{//(vetor[meio]->chaveBusca) < chave
        ini = meio+1;

        return busca_bin_rec_str(vetor, ini, fim, chave);
    }
}