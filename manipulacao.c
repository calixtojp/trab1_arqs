#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "manipulacao.h"
#include "auxiliares.h"
#include "arq_dados.h"
#include "arq_indice.h"

#define MAX_NAME_ARQ 50

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

struct ConexaoReg{
    char campoIndexado[MAX_NAME_ARQ];
    char tipoDado[MAX_NAME_ARQ];
    long int cursorDados;
    long int cursorIndex;
    dados_t *dado;
    dados_indx_int_t *index_int;
    dados_indx_str_t *index_str;
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
    return arq_index_main;
}

ConexaoReg_t *alocar_cnx(ArqIndex_t *arq_index){
    ConexaoReg_t *cnx = malloc(sizeof(ConexaoReg_t));
    cnx->dado = alocar_dados();
    strcpy(cnx->campoIndexado, arq_index->campoIndexado);
    strcpy(cnx->tipoDado, arq_index->tipoDado);
    return cnx;
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

char *getCampoIndexado(ArqIndex_t *arq_index){
    return arq_index->campoIndexado;
}

int get_nroRegValidos(ArqDados_t *arq_dados){
    //Retorna o número de registros válidos:
    //     nroRegArq - nroRegRem
    int resultado = 0;
    resultado += get_nroRegArq(arq_dados->cabecalhoDados);
    resultado -= get_nroRegRem(arq_dados->cabecalhoDados);
    return resultado;
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
    if(strcmp(arq_index->tipoDado, "inteiro") == 0){
        arq_index->vet_indx_int = aloc_vet_DadoInt(nroRegValidos);
    }else if(strcmp(arq_index->tipoDado, "string") == 0){
        arq_index->vet_indx_str = aloc_vet_DadoStr(nroRegValidos);
    }else{
        printf("Não tem esse tipo\n");
    }
}

void escreve_dadoIndex(ConexaoReg_t *cnx_reg, long int bos_reg_ant){
    /*
        Escreve o byteOffSet do campo indexado do registro contido
    na conexão (cnx_reg) levando em conta o ByteOffSet do final do registro 
    anterior (ou inicio no próprio registro contigo em cnx_reg).
        Além disso, escreve em cnx_reg->dado_indx_x o valor contido
    no campo indexado
    */
    printf("campoIndexado:%s\n", cnx_reg->campoIndexado);

    if(strcmp(cnx_reg->tipoDado, "inteiro")==0){
        cnx_reg->index_int = malloc(sizeof(dados_indx_int_t*));
    }else if(strcmp(cnx_reg->tipoDado, "string")==0){
        cnx_reg->index_str = malloc(sizeof(dados_indx_str_t*));
    }


    bos_reg_ant += 1;//acrescento o byte do campo "removido"
    if(strcmp(cnx_reg->campoIndexado, "idCrime")==0){
        int idCrime = getIdCrime(cnx_reg->dado);
        set_CnxInt(cnx_reg->index_int, bos_reg_ant, idCrime);
        return;
    }else{
        bos_reg_ant += 4;//passo os bytes do campo "idCrime"
    }

    if(strcmp(cnx_reg->campoIndexado, "dataCrime")==0){
        char dataCrime[10];
        getDataCrime(cnx_reg->dado, dataCrime);
        set_CnxStr(cnx_reg->index_str, bos_reg_ant, dataCrime);
        return;
    }else{
        bos_reg_ant += 10;//passo os bytes do campo "dataCrime"
    }

    if(strcmp(cnx_reg->campoIndexado, "numeroArtigo")==0){
        int numeroArtigo = getNumeroArtigo(cnx_reg->dado);
        set_CnxInt(cnx_reg->index_int, bos_reg_ant, numeroArtigo);
        return;
    }else{
        bos_reg_ant += 4;//passo os bytes do campo "numeroArtigo"
    }

    if(strcmp(cnx_reg->campoIndexado, "marcaCelular")==0){
        char marcaCeular[12];
        getMarcaCelular(cnx_reg->dado, marcaCeular);
        set_CnxStr(cnx_reg->index_str, bos_reg_ant, marcaCeular);
        return;
    }else{
        printf("Erro na escrita do dado_index\n");
    }
}

int getDado(ConexaoReg_t *cnx_reg, ArqDados_t *arq_dados, int pular){
    /*
        -Pula 'pular' registros e lê o próximo registro de dados, 
    armazenando-o em 'cnx_reg'.
        -Retorna 1 se conseguiu realizar a leitura e 0 caso contrário.
    */

    //Primeiro vou pular os registros desejados.
    dados_t *registro_atual = alocar_dados();
    int cont_pulados = 0;//Conta quantos registros eu pulei.
    while(cont_pulados < pular){
        if(ler_bin_registro(registro_atual, arq_dados->arqDados)){
            //Se consegui ler o registro
            if(get_registro_removido(registro_atual)!=1){
                //Se o registro não foi removido, ou seja
                //é um registro válido.
                ++cont_pulados;
            }
        }else{
            //Se não consegui ler o registro, retorno 0
            return 0;
        }
    }

    //Agora faço a leitura do próximo registro válido
    //e armazeno-o em cnx_reg.
    int conseguiu_ler;
    long int byteOffSetAnt = 0;//byteOffSet do inicio do registro anterior
    byteOffSetAnt += 17; //incremento o cabeçalho do arq_dados
    conseguiu_ler = ler_bin_registro(registro_atual, arq_dados->arqDados);
    while(conseguiu_ler == 1){
        //Enquanto consegue ler
        if(get_registro_removido(registro_atual) != 1){
            //Se esse é um registro válido

            //faço a cópia do dado
            copia_registro(cnx_reg->dado, registro_atual);
            printf("consegui fazer a copia\n");
            //anoto o byteOffSet do campoIndexado e o valor do
            //campoIndexado
            escreve_dadoIndex(cnx_reg, byteOffSetAnt);

            return 1;
        }
        byteOffSetAnt = ftell(arq_dados->arqDados);
        conseguiu_ler = ler_bin_registro(registro_atual, arq_dados->arqDados);
    }


    //Se cheguei no fim (ou seja, saí do laço) e não achei
    //um registro válido, retorno 0
    return 0;
}

void escreverVetIndex(ConexaoReg_t *cnx_reg, ArqIndex_t *arq_index, int pos){
    if(strcmp(arq_index->tipoDado, "inteiro")==0){
        copiaDadoIndex_int(arq_index->vet_indx_int[pos], cnx_reg->index_int);
        
    }else if(strcmp(arq_index->tipoDado, "string")==0){
        copiaDadoIndex_str(arq_index->vet_indx_str[pos], cnx_reg->index_str);
    }else{
        printf("campo indexado não encontrado\n");
    }
}