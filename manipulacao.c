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
    int tipoDadoInt;/*tu ja tem o campo tipo de dado mano*/
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
    switch(arq_index->tipoDadoInt){
        case 0:
            return arq_index->vet_indx_int;
            break;
        case 1:
            return arq_index->vet_indx_str;
            break;
        default:
            printf("arq_index->tipoDadoInt não foi configurado\n");
            break;
    }
}

void *escolhe_indx_dado(ArqIndex_t *arq_index){
    switch(arq_index->tipoDadoInt){
        case 0:
            return alocDadoIndxInt();
            break;
        case 1:
            return alocDadoIndxStr();
            break;
        default:
            printf("arq_index->tipoDadoInt não foi configurado\n");
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

void ler_cabecalho_arq_index(ArqIndex_t *arq_index){
    //funcao que carrega na memoria primaria o registro de cabecalho de um arquivo de indice
    arq_index->cabecalhoIndex = ler_index_cabecalho(arq_index->arqIndex);
}

void ler_dados_arq_index(ArqIndex_t *arq_index){
    //funcao que carrega na memoria primaria os registros de dados de um arquivo de indice
    if(ehInteiro(arq_index->campoIndexado)){
        //se o campo for inteiro, preencho o vetor de dados inteiros
        arq_index->vet_indx_int = ler_index_dado_int(arq_index->arqIndex,arq_index->cabecalhoIndex);
    }else{
        //se o campo nao for inteiro, entao ele eh string. Entao preencho o vetor de dados string
        arq_index->vet_indx_str = ler_index_dado_str(arq_index->arqIndex,arq_index->cabecalhoIndex);
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
        free(cabecalho);
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
    arq_dados->cabecalhoDados = ler_dados_cabecalho(arq_dados->arqDados);
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
        arq_index->tipoDadoInt = 0;
    }else if(strcmp(arq_index->tipoDado, "string") == 0){
        arq_index->vet_indx_str = aloc_vet_indx_DadoStr(nroRegValidos);
        arq_index->tipoDadoInt = 1;
    }else{
        printf("Não tem esse tipo\n");
    }
}

int indexaRegistro(ArqDados_t *arq_dados, ArqIndex_t *arq_index, int posReg, long int *byteOffSetAnt){
    dados_t *atual_reg = alocar_dados();
    int liRegValido=0;//0 até que se prove o contrário
    int consegui_ler;

    //tipo de dado que será usado (int ou str)
    int tipDado = arq_index->tipoDadoInt;
 
    consegui_ler = ler_bin_registro(atual_reg, arq_dados->arqDados);
    while(consegui_ler){
        //Se consegui ler, devo conferir se esse é um registro
        //indexável - isto é, não removido e com campo válido.

        int eh_removido = get_registro_removido(atual_reg); 
        typedef int (*FncCampoNulo) (void*);
        FncCampoNulo fncsCampoNulo[] = {
            campoNulo_int,
            campoNulo_str
        };
        typedef void* (*FncGetCampoIndexIndexado) (dados_t*, char*);
        FncGetCampoIndexIndexado fncsGetCampo[] = {
            getCampoInt,
            getCampoStr
        };

        void *campoIndexado = malloc(sizeof(void));
        int eh_campo_nulo;

        //obtenho o campo indexado (int ou str)
        campoIndexado = fncsGetCampo[tipDado](atual_reg, arq_index->campoIndexado);

        //sejo se o campo é nulo ou não
        eh_campo_nulo = fncsCampoNulo[tipDado](campoIndexado);

        if(eh_removido || eh_campo_nulo){
            //Se não é um registro indexável, tento ler o próximo
            *byteOffSetAnt = (*byteOffSetAnt) + len_reg_dados(atual_reg);
            consegui_ler = ler_bin_registro(atual_reg, arq_dados->arqDados);
        }else{
            //Se consegui ler, devo indexá-lo
            liRegValido=1;

            //Crio o dado
            void *dadoIndexado = malloc(sizeof(void));
            void *vetIndx = malloc(sizeof(void));

            typedef void (*FncSetDadoIndx) (void*, long int, void*);
            FncSetDadoIndx fncsSetDadoIndx[] = {
                setDadoIndxInt,
                setDadoIndxStr
            };
            typedef void (*FncSetVetIndx) (void*, int, void*);
            FncSetVetIndx fncsSetVetIndx[] = {
                setVetIndx_int,
                setVetIndx_str
            };

            //escolho entre dado e o vetor para int ou str
            dadoIndexado = escolhe_indx_dado(arq_index);
            vetIndx = escolhe_vet_indx(arq_index);

            //escrevo as informações (campo indexado e byteOffSet)
            fncsSetDadoIndx[tipDado](dadoIndexado, *byteOffSetAnt, campoIndexado); 

            //agora escrevo o dado no vetor em RAM
            fncsSetVetIndx[tipDado](vetIndx, posReg, dadoIndexado);

            //agora incremento o contador de byteOffSet
            *byteOffSetAnt += len_reg_dados(atual_reg);
            //Como a escrita ocorreu bem, retorno 1
            return 1;
        }
    }

    if(liRegValido == 0){//significa que não li nenhum registro indexável
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

    // printf("antes de ordenar:\n");
    // fncs_MostraVet[arq_index->tipoDadoInt](vet_indx, qntd_reg);

    fncs_ordena_vet_indx[arq_index->tipoDadoInt](vet_indx, qntd_reg);

    // printf("depois de ordenar:\n");
    // fncs_MostraVet[arq_index->tipoDadoInt](vet_indx, qntd_reg);
}

void escreveVetIndex(ArqIndex_t *arq_index, int inicio, int fim){
    //Posiciono no local de escrita correto
    typedef int (*FncGetTamDadoIndx) (void);
    FncGetTamDadoIndx fncsGetTamDadoIndx[] = {
        getTamDadoIndx_int,
        getTamDadoIndx_str
    };

    int tamCabecalho = getTamCabecalhoIndx();
    int tamDado = fncsGetTamDadoIndx[arq_index->tipoDadoInt]();

    fseek(
        arq_index->arqIndex,
        tamCabecalho + (inicio*tamDado),
        SEEK_SET
    );

    //escolho que tipo de dado será escrito
    void *vet_indx = malloc(sizeof(void));
    vet_indx = escolhe_vet_indx(arq_index);
    typedef void (*FncEscreveIndx) (FILE*, void*, int);
    FncEscreveIndx fncsEscreveIndx[] = {
        escreveVetIndx_int,
        escreveVetIndx_str
    };

    //escrevo efetivamente o dado
    for(int cont = inicio; cont <= fim; ++cont){
        fncsEscreveIndx[arq_index->tipoDadoInt](arq_index->arqIndex, vet_indx, cont);    
    }
}

void terminaEscritaIndex(ArqIndex_t *arq_index, int qtndReg){
    //volto pára o início do arquivo e escrevo o cabeçalho final
    setCabecalhoIndex(
        arq_index->cabecalhoIndex,
        '1',
        qtndReg
    );
    fseek(arq_index->arqIndex, 0, SEEK_SET);
    escreveCabecalhoIndex(arq_index->arqIndex, arq_index->cabecalhoIndex);
}

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

void percorrer_index(long int (*get_byteOffset)(void *,int), int pos_prim, int qtd_reg_val, void *vetor, 
ArqDados_t *arq_dados,char **vet_nomes, char **vet_vals_str, int *vet_vals_int, int qtd_crit){

    if(pos_prim == -1){
        //como nao foi encontrado nenhum registro que satisfaz a busca, informo que o registro nao existe
        printf("Registro inexistente.\n");
    }else{
        /*Como existe pelo menos 1 registro que satisfaz a busca, percorro o vet_indx_int para todos os 
        registros que satisfazem o criterio de busca do campo indexado e testo os outros criterios de busca*/

        //vetor com todos os registros que serão printados. Inicialmente tem tamanho 1
        dados_t **vetor_registros = alocar_vet_dados(1);
        //contador de quantos elementos há no vetor de registros
        int cont_reg_vet = 0;

        for(int i=0; i<qtd_reg_val; i++){
            //com a pos do primeiro e a qtd de registros, eu pego o byteoffset de todos eles
            long int byteoffset = get_byteOffset(vetor,pos_prim+i);//PONTEIRO DE FUNCAO AQUI 2X
            
            //pra cada byteoffset, eu checo todos os criterios e retorno um registro de dados
            dados_t *registro = alocar_dados();
            if(testar_byteOffset(byteoffset, arq_dados->arqDados, vet_nomes, vet_vals_str, vet_vals_int, qtd_crit,registro)){
                //se o registro satisfaz todos os criterios, adiciono ele no vetor de registros que serão printados 
                vetor_registros = realloc(vetor_registros, (cont_reg_vet+1)*sizeof(dados_t *));
                vetor_registros[cont_reg_vet] = registro;
                cont_reg_vet++;
            }
        }

        if(cont_reg_vet == 0){
            //se apos testar os outros criterios, nenhum registro satisfaz a busca, informo que o registro nao existe
            printf("Registro inexistente.\n");
        }else{
            //se o numero de registros que satisfaz a busca é pelo menos 1, printo ele
            print_registros(vetor_registros,cont_reg_vet);
            desalocar_vet_dados(vetor_registros,cont_reg_vet);
        }
    }
}

void busca_bin_index(ArqIndex_t *arq_index, ArqDados_t *arq_dados, int pos_chave, char **vet_nomes, 
char **vet_vals_str, int *vet_vals_int, int qtd_crit){

    if(comparar_strings(arq_index->tipoDado,"inteiro")==0){
        //se o campo indexado for do tipo inteiro, faço uma busca binária para valores inteiros

        int qtd_reg_val = 0; //guarda o numero de registros que satisfazem o criterio de busca do arquivo de indice

        int pos_prim = busca_bin_int(arq_index->vet_indx_int,arq_index->cabecalhoIndex,vet_vals_int[pos_chave],&qtd_reg_val);
        //'busca_bin_int' retorna a posição do primeiro registro que satisfaz o criterio de busca no vet_indx_int,
        //caso nenhum satisfaça, retorna -1
        //por referencia, passa o numero de registros que satisfazem o criterio de busca para 'qtd_reg_val'

        //com a posição do primeiro e o numero de registros, percorro o vetor vet_indx_int
        percorrer_index(get_byteOffset_int,pos_prim,qtd_reg_val,arq_index->vet_indx_int,arq_dados,
        vet_nomes,vet_vals_str,vet_vals_int,qtd_crit);

    }else{    
        //se o campo indexado nao for inteiro, entao é string. Assim, faço busca binária para strings

        int qtd_reg_val = 0; //guarda o numero de registros que satisfazem o criterio de busca do arquivo de indice

        //como, no arquivo de index, as strings sao todas truncadas, deve-se tratar a chave de busca
        char *chave = truncar(vet_vals_str[pos_chave]);

        int pos_prim = busca_bin_str(arq_index->vet_indx_str,arq_index->cabecalhoIndex,chave,&qtd_reg_val);
        free(chave);
        //'busca_bin_str' retorna a posição do primeiro registro que satisfaz o criterio de busca no vet_indx_str,
        //caso nenhum satisfaça, retorna -1
        //por referencia, passa o numero de registros que satisfazem o criterio de busca para 'qtd_reg_val'

         //com a posição do primeiro e o numero de registros, percorro o vetor vet_indx_str
        percorrer_index(get_byteOffset_str,pos_prim,qtd_reg_val,arq_index->vet_indx_str,arq_dados,
        vet_nomes,vet_vals_str,vet_vals_int,qtd_crit);
    }
}

void busca_seq_dados(ArqDados_t *arq_dados, int m, char **vet_vals_str, int *vet_vals_int){
    
    printf("Nao existe arquivo index\n");
}

int get_nroRegIndex(ArqIndex_t *arq_index){
    return get_qtdReg(arq_index->cabecalhoIndex);
}

void mostrar_arq_index(ArqIndex_t *arq_index){
    typedef void (*FncMostraTipo) (void*, int);
    FncMostraTipo fncs_MostraVet[] = {
        mostraVetInt,
        mostraVetStr
    }; 

    void *vet_gen = malloc(sizeof(void));
    vet_gen = escolhe_vet_indx(arq_index);

    fncs_MostraVet[arq_index->tipoDadoInt](vet_gen, get_qtdReg(arq_index->cabecalhoIndex));
}