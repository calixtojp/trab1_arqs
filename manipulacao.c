#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "manipulacao.h"
#include "auxiliares.h"
#include "arq_dados.h"
#include "arq_indice.h"

#define MAX_NOME_ARQ 50
#define TAM_MAX_NOME 50
#define TAM_MAX_VALOR 50

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

struct InfoBusca{
    int qtd_crit;
    char **nomes;
    char **vals_str;
    int *vals_int;
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

int existe_index(InfoBusca_t *criterios, ArqIndex_t *arq_index){
    /*Função que, se o vetor de nomes (lido da entrada da funcionalidade [4]) 
    contiver o nome do campo indexado no arquivo de index, retorna o índice dessa string no vetor. 
    Caso contrário, retorna -1.*/

    /*Essa função determina se deve ser realizada 
    busca binária no arquivo de index ou sequencial no de dados.*/

    for(int i=0; i<criterios->qtd_crit; i++){
        if(strcmp(arq_index->campoIndexado,criterios->nomes[i])==0){
            return i;
        }
    }
    return -1;
}

long int *percorrer_index(long int (*get_byteOffset)(void *,int), int pos_prim, int qtd_reg_val, void *vetor, 
                    ArqDados_t *arq_dados, InfoBusca_t *criterios, int *cont_reg_vet){

    //vetor com os byteOffsets dos registros que satisfazem os criterios de busca. Inicialmente tem tamanho 1
    long int *vetor_byteOffset = malloc(sizeof(long int));
    
    if(pos_prim != -1){
        /*Como existe pelo menos 1 registro que satisfaz a busca, percorro o vet_indx_int para todos os 
        registros que satisfazem o criterio de busca do campo indexado e testo os outros criterios de busca*/

        for(int i=0; i<qtd_reg_val; i++){
            //com a pos do primeiro e a qtd de registros, eu pego o byteoffset de todos eles
            long int byteOffset = get_byteOffset(vetor,pos_prim+i);
            
            //pra cada byteoffset, eu checo todos os criterios
            if(testar_byteOffset(byteOffset, arq_dados->arqDados, criterios->nomes,criterios->vals_str,
                                criterios->vals_int, criterios->qtd_crit)){
                //se o registro satisfaz todos os criterios, adiciono o byteOffset dele no vetor de byteOffsets que será retornado
                vetor_byteOffset = realloc(vetor_byteOffset, (*cont_reg_vet+1)*sizeof(long int));
                vetor_byteOffset[*cont_reg_vet] = byteOffset;
                (*cont_reg_vet)++;
            }
        }
    }

    return vetor_byteOffset;
}

InfoBusca_t *ler_criterios_busca(){
    int m;
    scanf("%d",&m);

    InfoBusca_t *criterios = alocar_InfoBusca(m);

    for(int j=0; j<m; j++){
        scanf("%s",criterios->nomes[j]);
        if(ehInteiro(criterios->nomes[j])){
            //se o campo for um int
            scanf("%d",&criterios->vals_int[j]); //leio o valor

            //indico no vetor de valores string que o valor deve ser procurado no vetor de int
            strcpy(criterios->vals_str[j],"int");
        }else{
            //se nao for int, é string
            ler_aspas_string(criterios->vals_str[j]); //leio o valor

            //indico no vetor de valores int que o valor deve ser procurado no vetor de strings
            criterios->vals_int[j] = -1;
        }
    }

    return criterios;
}

InfoBusca_t *alocar_InfoBusca(int qtd_crit){
    InfoBusca_t *informacao = malloc(sizeof(InfoBusca_t));
    informacao->nomes = alocar_vetor_string(qtd_crit,TAM_MAX_NOME);
    informacao->vals_str = alocar_vetor_string(qtd_crit, TAM_MAX_VALOR);
    informacao->vals_int = malloc(sizeof(int)*qtd_crit);
    informacao->qtd_crit = qtd_crit;

    return informacao;
}

void desalocar_InfoBusca(InfoBusca_t *informacao){
    desalocar_vetor_string(informacao->nomes,informacao->qtd_crit);
    desalocar_vetor_string(informacao->vals_str,informacao->qtd_crit);
    free(informacao->vals_int);
    free(informacao);
}

void busca(ArqDados_t *arq_dados, ArqIndex_t *arq_index, int qtd_buscas){
    for(int i=1; i<=qtd_buscas; i++){
        printf("Resposta para a busca %d\n",i);

        InfoBusca_t *criterios = ler_criterios_busca();

        int cont_reg_vet = 0; //quantidade de registros que satisfazem os criterios de busca
        long int *resultado_busca = qual_busca(arq_dados,arq_index,criterios,&cont_reg_vet);

        //printar o vetor buscado
        printar_busca(arq_dados->arqDados,resultado_busca,cont_reg_vet);

        //Desalocar tipos utilizados    	
        desalocar_InfoBusca(criterios);
        free(resultado_busca);

        //reiniciar o ponteiro do arquivo de dados para o primeiro registro de dados (pulando o cabecalho)
        fseek(arq_dados->arqDados,len_cabecalho_dados(),SEEK_SET);
    }   
}

long int *qual_busca(ArqDados_t *arq_dados, ArqIndex_t *arq_index, InfoBusca_t *criterios, int *cont_reg_vet){
    //funcao que define se a busca sera binaria no arquivo de indice ou sequencial no arquivo de dados

    int existe = existe_index(criterios,arq_index);
    /*Se existe arquivo de indice para um dos campos a serem buscados,
    a variável 'existe' recebe o indice do nome desse campo no vet_nomes.
    Se não existe, a variável recebe -1.*/

    long int *vetor_byteOffset;//vetor com os byteOffsets dos registros que satisfazem os criterios de busca

    if(existe >= 0 ){
        /*se existe arquivo de index para um dos campos que se deseja 
        buscar, faz-se busca binária no arquivo de indice*/
        vetor_byteOffset = busca_bin_index(arq_index,arq_dados,existe,criterios,cont_reg_vet);
    }else{
        //se não, faz-se busca sequencial no arquivo de dados
        vetor_byteOffset = busca_seq_dados(arq_dados,criterios,cont_reg_vet);
    }

    return vetor_byteOffset;
}

long int *busca_bin_index(ArqIndex_t *arq_index, ArqDados_t *arq_dados, int pos_chave, InfoBusca_t *criterios, int *cont_reg_vet){

    long int *vetor_byteOffset;//vetor com os byteOffsets dos registros que satisfazem os criterios de busca

    if(comparar_strings(arq_index->tipoDado,"inteiro")==0){
        //se o campo indexado for do tipo inteiro, faço uma busca binária para valores inteiros

        int qtd_reg_val = 0; //guarda o numero de registros que satisfazem o criterio de busca do arquivo de indice

        int pos_prim = busca_bin_int(arq_index->vet_indx_int,arq_index->cabecalhoIndex,criterios->vals_int[pos_chave],&qtd_reg_val);
        //'busca_bin_int' retorna a posição do primeiro registro que satisfaz o criterio de busca no vet_indx_int,
        //caso nenhum satisfaça, retorna -1
        //por referencia, passa o numero de registros que satisfazem o criterio de busca para 'qtd_reg_val'

        //com a posição do primeiro e o numero de registros, percorro o vetor vet_indx_int
        vetor_byteOffset = percorrer_index(get_byteOffset_int,pos_prim,qtd_reg_val,arq_index->vet_indx_int,
                                        arq_dados,criterios,cont_reg_vet);
    }else{    
        //se o campo indexado nao for inteiro, entao é string. Assim, faço busca binária para strings

        int qtd_reg_val = 0; //guarda o numero de registros que satisfazem o criterio de busca do arquivo de indice

        int pos_prim = busca_bin_str(arq_index->vet_indx_str,arq_index->cabecalhoIndex,criterios->vals_str[pos_chave],&qtd_reg_val);
        //'busca_bin_str' retorna a posição do primeiro registro que satisfaz o criterio de busca no vet_indx_str,
        //caso nenhum satisfaça, retorna -1
        //por referencia, passa o numero de registros que satisfazem o criterio de busca para 'qtd_reg_val'

        //com a posição do primeiro e o numero de registros, percorro o vetor vet_indx_str
        vetor_byteOffset = percorrer_index(get_byteOffset_str,pos_prim,qtd_reg_val,arq_index->vet_indx_str,
                                        arq_dados,criterios, cont_reg_vet);
    }

    return vetor_byteOffset;
}

long int *busca_seq_dados(ArqDados_t *arq_dados, InfoBusca_t *criterios, int *cont_reg_vet){
    
    long int *vetor_byteOffset = malloc(sizeof(long int));

    long int byteOffset_atual = len_cabecalho_dados();
    
    dados_t *registro = alocar_dados();
        
    int consegui_ler = ler_bin_registro(registro, arq_dados->arqDados);
    while(consegui_ler){

        //Se consegui ler, devo conferir se esse é um registro removido
        int eh_removido = get_registro_removido(registro);

        if(eh_removido == 0){
            //se o registro nao é removido, avalio os criterios de busca
            if(testar_criterios(registro,criterios->nomes,criterios->vals_str,criterios->vals_int,criterios->qtd_crit)){
                //se o registro satisfaz todos os criterios, adiciono ele no vetor de byteOffsets que serão printados 
                vetor_byteOffset = realloc(vetor_byteOffset, (*cont_reg_vet+1)*sizeof(long int));
                vetor_byteOffset[*cont_reg_vet] = byteOffset_atual;
                (*cont_reg_vet)++;
            }
        }

        byteOffset_atual += len_reg_dados(registro);
        desalocar_registro(registro);
        registro = alocar_dados();
        consegui_ler = ler_bin_registro(registro, arq_dados->arqDados);
    }
    free(registro);

    return vetor_byteOffset;
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

int testar_status(ArqIndex_t *arq_index, ArqDados_t *arq_dados){
    //funcao que confere o status do arquivo de indice e de dados
    //se os dois estao consistentes, retorna 2
    //se um está inconsistente, retorna 1
    //se os dois estao inconsistentes, retorna 0

    int retorno = 0;

    retorno += testar_status_dados(arq_dados->cabecalhoDados);
    retorno += testar_status_indx(arq_index->cabecalhoIndex);

    return retorno;
}