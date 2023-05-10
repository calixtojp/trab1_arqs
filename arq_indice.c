#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "arq_indice.h"
#include "auxiliares.h"

#define TAM_CBCL_INDX 5
#define TAM_CAMP_STR 12
#define TAM_DADO_INDX_STR 20
#define TAM_DADO_INDX_INT 12

//Cabeçalho do arquivo de indice
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
    printf("entrei em ler_index_cabecalho\n");

	cabecalho_indx_t *cabecalho_retorno = alocar_cbl_indx();

    char status;
    int qtdReg;
    if(fread(&status,sizeof(char),1,arq)!=1){
        printf("não consegui ler o status\n");
    }

    if(fread(&qtdReg,sizeof(int),1,arq)!=1){
        printf("Não consegui ler a qtdReg\n");
    }
    
    cabecalho_retorno->status = status;
    cabecalho_retorno->qtdReg = qtdReg;

    printf("status:%c|qtdReg:%d\n", cabecalho_retorno->status, cabecalho_retorno->qtdReg);
	
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

void setCabecalhoIndex(cabecalho_indx_t *cabecalho, const char status, int qtdReg){
    cabecalho->status = status;
    cabecalho->qtdReg = qtdReg;
}

cabecalho_indx_t *get_cabecalho_indx(FILE *arqIndex){
    cabecalho_indx_t *cabecalho = alocar_cbl_indx();
	if(fread(&(cabecalho->status), sizeof(char), 1, arqIndex)!=1){
		printf("Falha no processamento do arquivo.\n");
		return NULL;
	}

	if(fread(&(cabecalho->qtdReg), sizeof(int), 1, arqIndex)!=1){
		printf("Falha no processamento do arquivo.\n");
		return NULL;
	}
}

void setDadoIndxInt(void *dado, long int byteOffSet, void *valor){
    dados_indx_int_t *dado_real = (dados_indx_int_t*)dado;
    int valor_real = *((int*)valor);
    dado_real->byteOffset = byteOffSet;
    dado_real->chaveBusca = valor_real;
}

void setDadoIndxStr(void *dado, long int byteOffSet, void *valor){
    dados_indx_str_t *dado_real = (dados_indx_str_t*)dado;
    char *valor_real = (char*)valor;
    dado_real->byteOffset = byteOffSet;
    strcpy(dado_real->chaveBusca, valor_real);
}

void setVetIndx_int(void *vet, int pos, void *dado){
    dados_indx_int_t **vet_real;
    dados_indx_int_t *dado_real;
    vet_real = (dados_indx_int_t**)vet;
    dado_real = (dados_indx_int_t*)dado;
    setDadoIndxInt(
        vet_real[pos],
        dado_real->byteOffset,
        &(dado_real->chaveBusca)
    );
}

void setVetIndx_str(void *vet, int pos, void *dado){
    dados_indx_str_t **vet_real;
    dados_indx_str_t *dado_real;
    vet_real = (dados_indx_str_t**)vet;
    dado_real = (dados_indx_str_t*)dado;
    setDadoIndxStr(
        vet_real[pos],
        dado_real->byteOffset,
        (dado_real->chaveBusca)
    );
}

void copiaDadoIndex_int(void *destino, void *origem){
    dados_indx_int_t *destino_real, *origem_real;
    destino_real = (dados_indx_int_t*)destino;
    origem_real = (dados_indx_int_t*)origem;
    destino_real->byteOffset = origem_real->byteOffset;
    destino_real->chaveBusca = origem_real->chaveBusca;
}

void copiaDadoIndex_str(void *destino, void *origem){
    dados_indx_str_t *destino_real, *origem_real;
    destino_real = (dados_indx_str_t*)destino;
    origem_real = (dados_indx_str_t*)origem;
    destino_real->byteOffset = origem_real->byteOffset;
    strcpy(destino_real->chaveBusca, origem_real->chaveBusca);
}

int getTamCabecalhoIndx(void){
    return TAM_CBCL_INDX;
}

int getTamDadoIndx_int(void){
    return TAM_DADO_INDX_INT;
}

int getTamDadoIndx_str(void){
    return TAM_DADO_INDX_STR;
}

int get_qtdReg(cabecalho_indx_t *cabecalho){
    return cabecalho->qtdReg;
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
    int valor_a, valor_b;
    long int byte_a, byte_b;
    valor_a = (*((dados_indx_int_t**)a))->chaveBusca;
    valor_b = (*((dados_indx_int_t**)b))->chaveBusca;
    byte_a = (*((dados_indx_int_t**)a))->byteOffset;
    byte_b = (*((dados_indx_int_t**)b))->byteOffset;
    
    if(valor_a == valor_b){
        return byte_a - byte_b;
    }else{
        return valor_a - valor_b;
    }
}

int compara_str(const void *a, const void *b){
    char str_a[TAM_CAMP_STR];
    char str_b[TAM_CAMP_STR];
    long int byte_a, byte_b;
    byte_a = (*((dados_indx_int_t**)a))->byteOffset;
    byte_b = (*((dados_indx_int_t**)b))->byteOffset;
    strcpy(str_a, (*((dados_indx_str_t**)a))->chaveBusca);
    strcpy(str_b, (*((dados_indx_str_t**)b))->chaveBusca);

    int cont=0;
    char curs_a, curs_b;
    do{
        curs_a = str_a[cont];
        curs_b = str_b[cont];
        int result = curs_a - curs_b;
        if(result != 0){
            return result;
        }else{
            cont++;
        }
    }while(curs_a!='$' && curs_b!='$' && cont<12);

    if(cont==0){
        return byte_a - byte_b;
    }
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
    fwrite(&cabecalho->qtdReg,sizeof(int), 1, arqIndex);
}

void escreveDadoIndx_int(FILE *arqIndex, void *dado){
    dados_indx_int_t *dado_real = (dados_indx_int_t*)dado;
    // printf("vou escrever o dado:");
    // mostraRegIndx_int(dado_real);
    fwrite(&(dado_real->chaveBusca), sizeof(int), 1, arqIndex);
    fwrite(&(dado_real->byteOffset), sizeof(long int), 1, arqIndex);
}

void escreveDadoIndx_str(FILE *arqIndex, void *dado){
    dados_indx_str_t *dado_real = (dados_indx_str_t*)dado;
    // printf("vou escrever o dado:");
    // mostraRegIndx_str(dado_real);
    fwrite((dado_real->chaveBusca), sizeof(char), TAM_CAMP_STR, arqIndex);
    fwrite(&(dado_real->byteOffset), sizeof(long int), 1, arqIndex);
}

void escreveVetIndx_int(FILE *arqIndex, void *vet_indx_int, int pos){
    dados_indx_int_t **vet_real;
    vet_real = (dados_indx_int_t**)vet_indx_int;
    escreveDadoIndx_int(arqIndex, vet_real[pos]);
}

void escreveVetIndx_str(FILE *arqIndex, void *vet_indx_str, int pos){
    dados_indx_str_t **vet_real;
    vet_real = (dados_indx_str_t**)vet_indx_str;
    escreveDadoIndx_str(arqIndex, vet_real[pos]);

}

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

int busca_bin_int(dados_indx_int_t **vetor, cabecalho_indx_t *cabecalho,int chave){
    int pos = busca_bin_rec_int(vetor,0,cabecalho->qtdReg,chave);
    
    if(pos != -1){
        /*Como há campos que podem ter valores iguais, a busca binaria retorna a posição de um dos valores que satisfaz a busca.
        No arquivo de index, esses valores estão ordenados, então quero o primeiro deles*/
        while(((pos-1)>=0) && ((vetor[pos-1]->chaveBusca) == (vetor[pos]->chaveBusca))){
            //o teste ((pos-1)>=0) deve ser feito para evitar segmentation fault caso 'pos' seja igual a zero
            pos--; //posição do primeiro registro que satisfaz a busca no vetor
        }
    }
    return pos; 
}

int busca_bin_rec_str(dados_indx_str_t **vetor, int ini, int fim, char *chave){
    if(ini > fim){
        return -1;
    } 

    int meio = (ini+fim)/2;

    if(strcmp(vetor[meio]->chaveBusca,chave)==0){

        return meio;
    }else if(strcmp(vetor[meio]->chaveBusca,chave)>0){//(vetor[meio]->chaveBusca) > chave
        fim = meio-1;

        return busca_bin_rec_str(vetor, ini, fim, chave);
    }else{//(vetor[meio]->chaveBusca) < chave
        ini = meio+1;

        return busca_bin_rec_str(vetor, ini, fim, chave);
    }
}

int busca_bin_str(dados_indx_str_t **vetor, cabecalho_indx_t *cabecalho, char *chave){
    int pos = busca_bin_rec_str(vetor,0,cabecalho->qtdReg,chave);
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

void percorrer_vet_indx_int(dados_indx_int_t **vet_indx_int, int pos, int *vet_vals_int, char **vet_vals_str, int qtd_crit){
    do{
        // acessar_testar(vet_indx_int[pos]->byteOffset, vet_vals_int, vet_vals_str, qtd_crit);

        pos++;
    }while(vet_indx_int[pos]->chaveBusca == vet_indx_int[pos-1]->chaveBusca);
    //enquanto o registro atual tiver a mesma chave daquele que acabou de ser processado
}