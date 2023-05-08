#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "manipulacao.h"
#include "auxiliares.h"
#include "arq_dados.h"
#include "arq_indice.h"

#define MAX_NOME_ARQ 50

struct ArqDados{
    char nomeArqDados[MAX_NOME_ARQ];
    FILE *arqDados;
    cabecalho_t *cabecalhoDados;
};

struct ArqIndex{
    char campoIndexado[MAX_NOME_ARQ];
    char tipoDado[MAX_NOME_ARQ];
    char nomeArqIndex[MAX_NOME_ARQ];
    int qntReg;
    int globalTipoDado;
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
    switch(arq_index->globalTipoDado){
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

void ler_nome_arq_dados(ArqDados_t *arq_dados){
    int retorno_scanf = scanf(" %s", arq_dados->nomeArqDados); 
    if(retorno_scanf != 1){
        //erro de leitura
        exit(0);
    }
}

void ler_nome_arq_index(ArqIndex_t *arq_index){
    int retorno_scanf = scanf(" %s", arq_index->nomeArqIndex); 
    if(retorno_scanf != 1){
        //erro de leitura
        exit(0);
    }
}

void ler_campoIndexado(ArqIndex_t *arq_index){
    int retorno_scanf = scanf(" %s", arq_index->campoIndexado);
    if(retorno_scanf != 1){
        //erro de leitura
        exit(0);
    }
}

void ler_tipoDado(ArqIndex_t *arq_index){
    int retorno_scanf = scanf(" %s", arq_index->tipoDado);
    if(retorno_scanf != 1){
        //erro de leitura
        exit(0);
    }
}

void abrir_arq_dados(ArqDados_t *arq_dados, const char *tipo_leitura){
    arq_dados->arqDados = fopen(arq_dados->nomeArqDados, tipo_leitura);
    if(arq_dados->arqDados == NULL){
        exit(0);
    }
}

void abrir_arq_index(ArqIndex_t *arq_index, const char *tipo_leitura){
    arq_index->arqIndex = fopen(arq_index->nomeArqIndex, tipo_leitura);
    if(arq_index->arqIndex == NULL){
        exit(0);
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

void set_qtdReg(ArqIndex_t *arq_index, int qtdReg){
    arq_index->qntReg = qtdReg;
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
        arq_index->globalTipoDado = 0;
    }else if(strcmp(arq_index->tipoDado, "string") == 0){
        arq_index->vet_indx_str = aloc_vet_indx_DadoStr(nroRegValidos);
        arq_index->globalTipoDado = 1;
    }else{
        printf("Não tem esse tipo\n");
    }
}

int indexaRegistro(ArqDados_t *arq_dados, ArqIndex_t *arq_index, int pos_reg){
    int conseguiu_ler;
    //Flag que contém 1 se conseguiu ler e 0 no caso contrário

    long int byteOffSetAnt = ftell(arq_dados->arqDados);
    //Guarda o byteOffSet do começo do registro atual (atual_reg)
    
    dados_t *atual_reg = alocar_dados();
    //Guarda o registro atual

    dados_indx_int_t *indexInt = NULL;
    //Armazena o campo indexado (int) e o byteOffSet do mesmo
    dados_indx_str_t *indexStr = NULL;
    //Armazena o campo indexado (str) e o byteOffSet do mesmo

    //Aloco o tipo que será usado
    if(arq_index->vet_indx_int != NULL){
        indexInt = alocDadoIndxInt();
    }else if(arq_index->vet_indx_str != NULL){
        indexStr = alocDadoIndxStr();
    }else{
        printf("tipo de dado não encontrado\n");
    }

    conseguiu_ler = ler_bin_registro(atual_reg, arq_dados->arqDados);
    int liRegValido = 0;//1 se leu algum registro válido, 0 caso contrário
    while(conseguiu_ler == 1){
        //Enquanto consegue ler

        if(get_registro_removido(atual_reg) == 0){
            //Se esse é um registro válido,
            liRegValido = 1;

            long int bytOffSetCampIndex = byteOffSetAnt;
            //variável guardará o byteOffSet do campo indexado
            bytOffSetCampIndex += bytesAteCampoIndexado(atual_reg, arq_index->campoIndexado);
            //Calculo o byteOffSet do campo indexado

            if(indexInt != NULL){
                //Se não é nulo, é porque foi alocado e
                //se foi alocado, é porque esse será o dado usado
                int campoIndexado;
                campoIndexado = getCampoInt(atual_reg, arq_index->campoIndexado);
                setDadoIndxInt(indexInt, bytOffSetCampIndex, campoIndexado);
            }else if(indexStr != NULL){
                char *campoIndexado = alocarCampoIndexado();
                campoIndexado = getCampoStr(atual_reg, arq_index->campoIndexado);
                setDadoIndxStr(indexStr, bytOffSetCampIndex, campoIndexado);
            }else{
                printf("não alocou o indexX\n");
            }

            break;
            //Como já encontrei o campo indexado e o byteOffSet,
            //Posso sair do laço
        }
        byteOffSetAnt = ftell(arq_dados->arqDados);
        printf("byteOffSetAnt:%ld\n", byteOffSetAnt);
        conseguiu_ler = ler_bin_registro(atual_reg, arq_dados->arqDados);
    }

    //Se não conseguiu ler nenhum registro de dados válido,
    //é porque chegou no final do arquivo
    if(liRegValido == 0)
        return 0;

    //Já que a leitura foi concluída com sucesso,
    //tenho que escrever o registro lido no vetor de index
    printf("fiz a leitura do registro:\n");
    mostrar_campos(atual_reg);

    if(arq_index->vet_indx_int != NULL){
        //Se o tipo de dado é inteiro...
        copiaDadoIndex_int(arq_index->vet_indx_int[pos_reg], indexInt);
        printf("campos do reg que li:\n");
        mostraRegIndx_int(indexInt);
        printf("campos do reg que colei:\n");
        mostraRegIndx_int(arq_index->vet_indx_int[pos_reg]);
    }else if(arq_index->vet_indx_str != NULL){
        //Se o tipo de dado é string...
        copiaDadoIndex_str(arq_index->vet_indx_str[pos_reg], indexStr);
        printf("campos do reg que li:\n");
        mostraRegIndx_str(indexStr);
        printf("campos do reg que colei:\n");
        mostraRegIndx_str(arq_index->vet_indx_str[pos_reg]);
    }else{
        printf("não alocou o indexX\n");
    }

    return 1;
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
    fncs_MostraVet[arq_index->globalTipoDado](vet_indx, qntd_reg);

    fncs_ordena_vet_indx[arq_index->globalTipoDado](vet_indx, qntd_reg);

    printf("depois de ordenar:\n");
    fncs_MostraVet[arq_index->globalTipoDado](vet_indx, qntd_reg);
}

/*

typedef int (*FncTipoIndexado)(ArqIndex_t *arq_index);

int tipoIndexado_int (ArqIndex_t *arq_index){
    if(arq_index->vet_indx_int != NULL){
        return 1;
    }
}

int tipoIndexado_str (ArqIndex_t *arq_index){
    if(arq_index->vet_indx_str != NULL){
        return 1;
    }
}


FncEscreve escreve;

escreve = selecionaEscreve(arq_index);

for(int cont = inicio; inicio <= fim; ++cont){
    escreve(arq_index)
}

if(arq_index->vet_indx_int != NULL){
    escreve = selecionaEscreve(inteiro)
}else if(arq_index->vet_indx_str != NULL){
    for(int cont = inicio; cont <= fim; ++cont){
        escrevePija_str(arq_index->vet_indx_str, arq_index->arqIndex);
    }
}
*/

/*
void escreveVetIndex(ArqIndex_t *arq_index, int inicio, int fim){
    int escrevi = 1;//tem 1 se conseguiu escrever o dado, 0 caso contrário

    //Primeiro, escrevo que o status do arquivo de index é 
    //positivo (ou "1")
    setCabecalhoIndex(arq_index->cabecalhoIndex, '1');
    escreveCabecalhoIndex(arq_index->arqIndex, arq_index->cabecalhoIndex);


    if(arq_index->vet_indx_int != NULL){
        for(int cont = inicio; cont <= fim; ++cont){
            escrevePija_int(arq_index->vet_indx_int, arq_index->arqIndex);
        }
    }else if(arq_index->vet_indx_str != NULL){
        for(int cont = inicio; cont <= fim; ++cont){
            escrevePija_str(arq_index->vet_indx_str, arq_index->arqIndex);
        }
    }
}
*/

int existe_index(int m, char **vet_nomes, ArqIndex_t *arq_index){
    /*Função que, se o vetor de nomes (lido da entrada da funcionalidade [4]) 
    contiver o nome do campo indexado no arquivo de index, retorna o índice dessa string no vetor. 
    Caso contrário, retorna -1.*/

    /*Essa função determina se deve ser realizada 
    busca binária no arquivo de index ou sequencial no de dados.*/

    for(int i=0; i<m; i++){
        if(strcmp(arq_index->campoIndexado,vet_nomes[i])==0){
            return i;
        }
    }
    return -1;
}



void busca_bin_index(ArqIndex_t *arq_index, int pos, char **vet_vals_str, int *vet_vals_int){
    printf("Existe arquivo index\n");
    if(strcmp(arq_index->tipoDado,"inteiro")==0){
        printf("O campo indexado eh int\n");
        printf("Quero buscar o valor %d\n",vet_vals_int[pos]);
        // busca_bin_int(arq_index->cabecalhoIndex);
    }else{
        printf("O campo indexado eh string\n");
        printf("Quero buscar o valor %s\n",vet_vals_str[pos]);
    }
}

void busca_seq_dados(ArqDados_t *arq_dados, int m, char **vet_vals_str, int *vet_vals_int){
    printf("Nao existe arquivo index\n");
}
