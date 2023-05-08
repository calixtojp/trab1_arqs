#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "manipulacao.h"
#include "auxiliares.h"
#include "arq_dados.h"
#include "arq_indice.h"

#define MAX_NAME_ARQ 50

int globalTipoDado;

struct ArqDados{
    char nomeArqDados[MAX_NAME_ARQ];
    FILE *arqDados;
    cabecalho_t *cabecalhoDados;
};

struct ArqIndex{
    char campoIndexado[MAX_NAME_ARQ];
    char tipoDado[MAX_NAME_ARQ];
    char nomeArqIndex[MAX_NAME_ARQ];
    FILE *arqIndex;
    cabecalho_indx_t *cabecalhoIndex;
    dados_indx_str_t **vet_indx_str;
    dados_indx_int_t **vet_indx_int;
};

ArqDados_t *alocar_arq_dados(void){
    //Aloca o cabeçalho e o tipo ArqDados_t

    ArqDados_t *arq_dados_main;
    arq_dados_main = malloc(sizeof(ArqDados_t));

    arq_dados_main->cabecalhoDados = alocar_cabecalho_dados();

    return arq_dados_main;
}

ArqIndex_t *alocar_arq_index(void){
    //Aloca o cabeçalho e o tipo ArqIndex_t

    ArqIndex_t *arq_index_main;
    arq_index_main = malloc(sizeof(ArqIndex_t));

    arq_index_main->cabecalhoIndex = alocar_cbl_indx();
    arq_index_main->vet_indx_int = NULL;
    arq_index_main->vet_indx_str = NULL;
    return arq_index_main;
}

void *escolhe_vet_indx(ArqIndex_t *arq_index){
    switch(globalTipoDado){
        case 0:
            return arq_index->vet_indx_int;
            break;
        case 1:
            return arq_index->vet_indx_str;
            break;
        default:
            printf("globalTipoDado não foi configurado\n");
            break;
    }
}

void *escolhe_indx_dado(ArqIndex_t *arq_index){
    switch(globalTipoDado){
        case 0:
            return alocDadoIndxInt();
            break;
        case 1:
            return alocDadoIndxStr();
            break;
        default:
            printf("globalTipoDado não foi configurado\n");
            break;
    }
}

void ler_nome_arq_dados(ArqDados_t *arq_dados){
    int retorno_scanf = scanf(" %s", arq_dados->nomeArqDados); 
    if(retorno_scanf != 1){
        printf("ERRO: leitura do nome do arq_dados\n");
        exit(0);
    }
}

void ler_nome_arq_index(ArqIndex_t *arq_index){
    int retorno_scanf = scanf(" %s", arq_index->nomeArqIndex); 
    if(retorno_scanf != 1){
        printf("ERRO: leitura do nome do arq_index\n");
        exit(0);
    }
}

void ler_campoIndexado(ArqIndex_t *arq_index){
    int retorno_scanf;
    retorno_scanf = scanf(" %s", arq_index->campoIndexado);
    if(retorno_scanf != 1){
        printf("ERRO: leitura do campoIndexado\n");
        exit(0);
    }
}

void ler_tipoDado(ArqIndex_t *arq_index){
    int retorno_scanf;
    retorno_scanf = scanf(" %s", arq_index->tipoDado);
    if(retorno_scanf != 1){
        printf("ERRO: leitura do tipoDado\n");
        exit(0);
    }
}

void abrir_arq_dados(ArqDados_t *arq_dados, const char *tipo_leitura){
    arq_dados->arqDados = fopen(arq_dados->nomeArqDados, tipo_leitura);
    if(arq_dados->arqDados == NULL){
        printf("erro na abertura do arqDados\n");
        exit(0);
    }
}

void abrir_arq_index(ArqIndex_t *arq_index, const char *tipo_leitura){
    arq_index->arqIndex = fopen(arq_index->nomeArqIndex, tipo_leitura);
    if(arq_index->arqIndex == NULL){
        printf("erro na abertura do arqIndex\n");
        exit(0);
    }

    if(strcmp(tipo_leitura, "wb")==0){
        //Se o tipo de leitura for "wb", então escrevo o
        //cabeçalho inicial
        cabecalho_indx_t *cabecalho = alocar_cbl_indx();
        setCabecalhoIndex(cabecalho, '0', 0);
        escreveCabecalhoIndex(arq_index->arqIndex, cabecalho);
    }
} 

void desalocar_ArqDados(ArqDados_t *arq_dados){
    free(arq_dados->cabecalhoDados);
    free(arq_dados);
}

void desalocar_ArqIndex(ArqIndex_t *arq_index){
    free(arq_index->cabecalhoIndex);
    free(arq_index);
}

void fechar_arq_dados(ArqDados_t *arq_dados){
    fclose(arq_dados->arqDados);
}

void fechar_arq_index(ArqIndex_t *arq_index){
    fclose(arq_index->arqIndex);
}

void ler_cabecalho_dados(ArqDados_t *arq_dados){
    arq_dados->cabecalhoDados = ler_bin_cabecalho(arq_dados->arqDados);
}

int ler_cabecalho_index(ArqIndex_t *arq_index){
    arq_index->cabecalhoIndex = get_cabecalho_indx(arq_index->arqIndex);
    if(arq_index->cabecalhoIndex == NULL){
        //Se não consegui ler o cabeçalho, retorno 0
        return 0;
    }else{
        return 1;
    }
}

int getTamCabecalhoDados(ArqDados_t *arq_dados){
    return len_cabecalho_dados();
}

void mostrar_cabecalhoDados(ArqDados_t *arq_dados){
    mostrar_cabecalho_dados(arq_dados->cabecalhoDados);
}

void confere_arq_dados(ArqDados_t *arq_dados){
    //verifica-se a disponibilidade do arquivo
    if(status_disponivel(arq_dados->cabecalhoDados) == '0'){
        printf("Falha no processamento do arquivo.\n");
        exit(0);
    }

    //verifica-se a existência de registros válidos 
    if(!existem_registros(arq_dados->cabecalhoDados)){
        printf("Registro inexistente.\n");
        sair_fechando(arq_dados->arqDados);
    }
}

int get_nroRegValidos(ArqDados_t *arq_dados){
    //Retorna o número de registros válidos:
    //     nroRegArq - nroRegRem
    int resultado = 0;
    resultado += get_nroRegArq(arq_dados->cabecalhoDados);
    resultado -= get_nroRegRem(arq_dados->cabecalhoDados);
    return resultado;
}

char *getNomeArqIndex(ArqIndex_t *arq_index){
    return arq_index->nomeArqIndex;
}

void alocar_vet_index(ArqIndex_t *arq_index, unsigned int nroRegValidos){
    /*
            Aloca, na memória primária, um vetor que guardará os registros
        do arquivo de index.
            O vetor ficará armazenado no vet_indx_str ou vet_indx_int, a 
        depender o tipo de campo armazenado utilizado.
            Essa função leva em consideração o número de registros válidos 
        (que serão utilizados para indexação).
    */

    int tam;
    if(strcmp(arq_index->tipoDado, "inteiro") == 0){
        arq_index->vet_indx_int = aloc_vet_indx_DadoInt(nroRegValidos);
        globalTipoDado = 0;
    }else if(strcmp(arq_index->tipoDado, "string") == 0){
        arq_index->vet_indx_str = aloc_vet_indx_DadoStr(nroRegValidos);
        globalTipoDado = 1;
    }else{
        printf("Não tem esse tipo\n");
    }
}

int indexaRegistro(ArqDados_t *arq_dados, ArqIndex_t *arq_index, int posReg, long int *byteOffSetAnt){
    dados_t *atual_reg = alocar_dados();
    int liRegValido=0;//0 até que se prove o contrário
    int consegui_ler;
    long int byteOffSetCampIndx;//byteOffSet do campo indexado
    
    //Faço loop enquanto conseguir ler (não cheguei no fim do arquivo)
    //e enquanto não li um registro válido (pois devo pular registros removidos)

    consegui_ler = ler_bin_registro(atual_reg, arq_dados->arqDados);

    while(consegui_ler==1 && liRegValido==0){
        // printf("li o registro:\n");
        mostrar_campos(atual_reg);

        liRegValido = get_registro_removido(atual_reg);
        // printf("liRegValido:%d\n",liRegValido);
        if(liRegValido == 0){
            printf("esse era um registro válido\n");

            //byteOffSet do campo indexado = 
            //byteOffSet do inicio do registro + bytes até o campo indexado
            byteOffSetCampIndx = *byteOffSetAnt;
            byteOffSetCampIndx += bytesAteCampoIndexado(atual_reg, arq_index->campoIndexado);

            typedef void (*FncSetDado) (void*, long int, void*);
            FncSetDado fncsSetDado[] = {
                setDadoIndxInt,
                setDadoIndxStr
            };

            typedef void* (*FncGetCampo) (dados_t*, char*);
            FncGetCampo fncsGetCampo[] = {
                getCampoInt,
                getCampoStr
            };

            typedef void (*FncSetVetIndx) (void*, int, void*);
            FncSetVetIndx fncsSetVetIndx[] = {
                setVetIndx_int,
                setVetIndx_str
            };

            void *dadoIndx = malloc(sizeof(void));
            void *vet_indx = malloc(sizeof(void));
            void *campoIndexado = malloc(sizeof(void));
            vet_indx = escolhe_vet_indx(arq_index);
            dadoIndx = escolhe_indx_dado(arq_index);

            campoIndexado = fncsGetCampo[globalTipoDado](atual_reg, arq_index->campoIndexado);
            fncsSetDado[globalTipoDado](dadoIndx, byteOffSetCampIndx, campoIndexado);
            fncsSetVetIndx[globalTipoDado](vet_indx, posReg, dadoIndx);

            //Depois de tudso dou return
            *byteOffSetAnt = *(byteOffSetAnt) + len_reg_dados(atual_reg);//IMPLEMENTAR
            return 1;
        }

        //DEIXAR POR ÚLTIMO
        *byteOffSetAnt = *(byteOffSetAnt) + len_reg_dados(atual_reg);//IMPLEMENTAR
        consegui_ler = ler_bin_registro(atual_reg, arq_dados->arqDados);
    }

    if(liRegValido == 0){
        //Significa que passei pelo arquivo de dados até o fim e
        //não consegui ler 
        return 0;
    }
}



void ordenaVetIndex(ArqIndex_t *arq_index, int qntd_reg){
    //Ordena o vetor de index carregado em RAM
    typedef void (*FncOrdemTipo) (void*, int);
    FncOrdemTipo fncs_ordena_vet_indx[] = {
        ordenaVetIndex_int,
        ordenaVetIndex_str
    };

    typedef void (*FncMostraTipo) (void*, int);
    FncMostraTipo fncs_MostraVet[] = {
        mostraVetInt,
        mostraVetStr
    }; 

    void *vet_indx = malloc(sizeof(void));
    vet_indx = escolhe_vet_indx(arq_index);

    printf("antes de ordenar:\n");
    fncs_MostraVet[globalTipoDado](vet_indx, qntd_reg);

    fncs_ordena_vet_indx[globalTipoDado](vet_indx, qntd_reg);

    printf("depois de ordenar:\n");
    fncs_MostraVet[globalTipoDado](vet_indx, qntd_reg);
}

void escreveVetIndex(ArqIndex_t *arq_index, int inicio, int fim){
    //Posiciono no local de escrita correto
    typedef int (*FncGetTamDadoIndx) (void);
    FncGetTamDadoIndx fncsGetTamDadoIndx[] = {
        getTamDadoIndx_int,
        getTamDadoIndx_str
    };

    int tamCabecalho = getTamCabecalhoIndx();
    int tamDado = fncsGetTamDadoIndx[globalTipoDado]();

    fseek(
        arq_index->arqIndex,
        tamCabecalho + inicio*tamDado,
        SEEK_SET
    );

    void *vet_indx = malloc(sizeof(void));
    vet_indx = escolhe_vet_indx(arq_index);
    typedef void (*FncEscreveIndx) (FILE*, void*, int);
    FncEscreveIndx fncsEscreveIndx[] = {
        escreveVetIndx_int,
        escreveVetIndx_str
    };

    for(int cont = inicio; cont <= fim; ++cont){
        fncsEscreveIndx[globalTipoDado](arq_index->arqIndex, vet_indx, cont);    
    }
}