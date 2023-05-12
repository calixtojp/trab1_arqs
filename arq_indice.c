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

void ler_index_cabecalho(FILE *arq, cabecalho_indx_t* cabecalho){ 
	//Lê e retorna um ponteiro para o cabeçalho do arquivo 
	erro(arq);

    char status;
    int qtdReg;
    if(fread(&status,sizeof(char),1,arq)!=1){
        mensagem_erro();
    }

    if(fread(&qtdReg,sizeof(int),1,arq)!=1){
        mensagem_erro();
    }
    
    cabecalho->status = status;
    cabecalho->qtdReg = qtdReg;
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

void *leDadoIndx_int(FILE *arqIndex){
    dados_indx_int_t *dado_retorno;
    dado_retorno = alocDadoIndxInt();
    int campoIndexado;
    long int byteOffSet;
    fread(&campoIndexado, sizeof(int), 1, arqIndex);
    fread(&byteOffSet, sizeof(long int), 1, arqIndex);

    dado_retorno->chaveBusca = campoIndexado;
    dado_retorno->byteOffset = byteOffSet;

    return dado_retorno;
}

void *leDadoIndx_str(FILE *arqIndex){
    dados_indx_str_t *dado_retorno;
    dado_retorno = alocDadoIndxStr();
    char campoIndexado[TAM_CAMP_STR];
    long int byteOffSet;
    fread(campoIndexado, sizeof(char), TAM_CAMP_STR, arqIndex);
    fread(&byteOffSet, sizeof(long int), 1, arqIndex);

    for(int i = 0; i < TAM_CAMP_STR; i++){
        dado_retorno->chaveBusca[i] = campoIndexado[i];
    }
    dado_retorno->byteOffset = byteOffSet;

    return dado_retorno;
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

int compara_dado_int(const void *a, const void *b){
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

int compara_dado_str(const void *a, const void *b){
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
    qsort(vetor_real, qntd_reg, sizeof(dados_indx_int_t*), compara_dado_int);
}

void ordenaVetIndex_str(void *vetor_generico, int qntd_reg){
    dados_indx_str_t **vetor_real = (dados_indx_str_t**)vetor_generico;
    qsort(vetor_real, qntd_reg, sizeof(dados_indx_str_t*), compara_dado_str);
}

void escreveCabecalhoIndex(FILE *arqIndex, cabecalho_indx_t *cabecalho){
    // printf("escrever o cabecalho:status(%c)|qtdReg(%d)|ftel(%ld)\n",
    //     cabecalho->status,
    //     cabecalho->qtdReg,
    //     ftell(arqIndex)
    // );

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
    //retorna -1 caso nenhum registro satisfaça os criterios de busca
    //Caso contrario, retorna a pos do primeiro registro que satisfaz a busca
    //por parametro ('qtd_reg_val'), retorna a quantidade de registros que satisfazem a busca 

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

int busca_bin_int(void *vetor, cabecalho_indx_t *cabecalho, void *chave, int *qtd_reg_val){
    //funcao que prepara para a busca binaria recursiva para tipo inteiro e trata o retorno
    dados_indx_int_t **vetor_real = (dados_indx_int_t**)vetor;
    int chave_real = *((int*)chave);

    int pos = busca_bin_rec(vetor,0,cabecalho->qtdReg,&chave_real,comparacao_vet_dados_indx_int_const);

    return tratamento(pos,qtd_reg_val,vetor,comparacao_vet_dados_indx_int);
}

int busca_bin_str(void *vetor, cabecalho_indx_t *cabecalho, void *chave, int *qtd_reg_val){

    dados_indx_str_t **vetor_real = (dados_indx_str_t**)vetor;
    char *chave_real = (char*)chave;

    //funcao que prepara para a busca binaria recursiva para tipo string e trata o retorno
    
    //como, no arquivo de index, as strings sao todas truncadas, deve-se tratar a chave de busca
    char *chave_truncada = truncar(chave, 12);

    int pos = busca_bin_rec(vetor_real,0,cabecalho->qtdReg,chave_truncada,comparacao_vet_dados_indx_str_const);

    free(chave_truncada);

    return tratamento(pos,qtd_reg_val,vetor_real,comparacao_vet_dados_indx_str);
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

int testar_status_indx(cabecalho_indx_t *cabecalho){
	//funcao que retorna 1 caso o arquivo esteja consistente e 0 caso esteja inconsistente
	if(cabecalho->status == '1'){
		return 1;
	}
	return 0;
}
