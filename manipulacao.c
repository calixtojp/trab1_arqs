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
    int tipoDadoInt;
    FILE *arqIndex;
    cabecalho_indx_t *cabecalhoIndex;
    dados_indx_str_t **vet_indx_str;
    dados_indx_int_t **vet_indx_int;
    
    void *vetTemp;
    int qtdReg_vetTemp;
    /*Os dois últimos campos são usados para fazer alterações no vetor de dados_indx (int ou str)
    Essa lógica foi adotada pois não é interessante mudar as informações do arquivo de índice 
    enquanto ele está sendo processado, dado que isso pode gerar diversos erros. Assim, depois de
    fazer todas as alterações, esses campos são copiados de volta para os originais*/
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
        case 1:
            return arq_index->vet_indx_str;
    }
}

void *escolhe_indx_dado(ArqIndex_t *arq_index){
    switch(arq_index->tipoDadoInt){
        case 0:
            return alocDadoIndxInt();
        case 1:
            return alocDadoIndxStr();
    }
}

void *escolhe_criterio_vet_vals(InfoBusca_t *criterios, int pos, int tipoDado){
    switch(tipoDado){
        case 0:
            return &(criterios->vals_int[pos]);
        case 1:
            return criterios->vals_str[pos];
        default:
            printf("tipoDado não configurado\n");
            break;
    }
}

void ler_nome_arq_dados(ArqDados_t *arq_dados){
    int retorno_scanf = scanf(" %s", arq_dados->nomeArqDados); 
    if(retorno_scanf != 1){
        mensagem_erro();
    }
}

void ler_nome_arq_index(ArqIndex_t *arq_index){
    int retorno_scanf = scanf(" %s", arq_index->nomeArqIndex); 
    if(retorno_scanf != 1){
        mensagem_erro();
    }
}

void ler_campoIndexado(ArqIndex_t *arq_index){
    int retorno_scanf = scanf(" %s", arq_index->campoIndexado);
    if(retorno_scanf != 1){
        mensagem_erro();
    }
}

void ler_tipoDado(ArqIndex_t *arq_index){
    int retorno_scanf;
    retorno_scanf = scanf(" %s", arq_index->tipoDado);
    if(retorno_scanf != 1){
        mensagem_erro();
    }

    if(strcmp(arq_index->tipoDado, "inteiro")==0){
        arq_index->tipoDadoInt = 0;
    }else if(strcmp(arq_index->tipoDado, "string")==0){
        arq_index->tipoDadoInt = 1;
    }
}

void ler_cabecalho_arq_index(ArqIndex_t *arq_index){
    //funcao que carrega na memoria primaria o registro de cabecalho de um arquivo de indice
    ler_index_cabecalho(arq_index->arqIndex, arq_index->cabecalhoIndex);
}

void ler_dados_arq_index(ArqIndex_t *arq_index){
    //funcao que carrega na memoria primaria os registros de dados de um arquivo de indice
    if(comparar_strings(arq_index->tipoDado,"inteiro")==0){
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
        mensagem_erro();
    }
}

void abrir_arq_index(ArqIndex_t *arq_index, const char *tipo_leitura){
    arq_index->arqIndex = fopen(arq_index->nomeArqIndex, tipo_leitura);
    if(arq_index->arqIndex == NULL){
        mensagem_erro();
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

char *getNomeArqDados(ArqDados_t *arq_dados){
    return arq_dados->nomeArqDados;
}

void levaFinalCursorDados(ArqDados_t *arq_dados){
    fseek(arq_dados->arqDados, 0, SEEK_END);
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

void realocar_vet_index(ArqIndex_t *arq_index, int original, int acrescimo){
    int tam_tot = original + acrescimo;
    if(strcmp(arq_index->tipoDado, "inteiro")==0){
        arq_index->vet_indx_int = (dados_indx_int_t**)realloc(arq_index->vet_indx_int,(sizeof(dados_indx_int_t*))*(tam_tot));

        for(int i = original; i < tam_tot; ++i){
            arq_index->vet_indx_int[i] = alocDadoIndxInt();
        }

    }else if(strcmp(arq_index->tipoDado, "string")==0){
        arq_index->vet_indx_str = realloc(arq_index->vet_indx_str,(sizeof(dados_indx_str_t*))*(tam_tot));

        for(int i = original; i < tam_tot; ++i){
            arq_index->vet_indx_str[i] = alocDadoIndxStr();
        }
    }
}

void realocar_vetIndxTemp(ArqIndex_t *arq_index, int original, int acrescimo){
    int tam_tot = original + acrescimo;
    if(strcmp(arq_index->tipoDado, "inteiro")==0){
        arq_index->vetTemp = realloc(arq_index->vetTemp,(sizeof(dados_indx_int_t*))*(tam_tot));

        dados_indx_int_t **vet_real = (dados_indx_int_t**)arq_index->vetTemp;
        for(int i = original; i < tam_tot; ++i){
            vet_real[i] = alocDadoIndxInt();
        }

    }else if(strcmp(arq_index->tipoDado, "string")==0){
        arq_index->vetTemp = realloc(arq_index->vet_indx_str,(sizeof(dados_indx_str_t*))*(tam_tot));

        dados_indx_str_t **vet_real = (dados_indx_str_t**)arq_index->vetTemp;
        for(int i = original; i < tam_tot; ++i){
            printf("antes\n");
            vet_real[i] = alocDadoIndxStr();
            printf("depois\n");
        }
    }
}

int indexaRegistro(ArqDados_t *arq_dados, ArqIndex_t *arq_index, int posReg, long int *byteOffSetAnt){
    dados_t *atual_reg = alocar_dados();
    int liRegValido=0;//0 até que se prove o contrário
    int consegui_ler;

    //tipo de dado que será usado (int ou str)
    int tipDado = arq_index->tipoDadoInt;
 
    //consegui_ler armazena -1 caso consiga ler e armazena o tamanho do registro caso consiga
    consegui_ler = ler_bin_registro(atual_reg, arq_dados->arqDados);
    while(consegui_ler){
        //Se consegui ler, devo conferir se esse é um registro
        //indexável - isto é, não removido e com campo válido.

        int eh_removido = get_registro_removido(atual_reg); 
        FncCampoNulo fncsCampoNulo[] = {campoNulo_int,campoNulo_str};
        FncGetCampoIndexIndexado fncsGetCampo[] = {getCampoInt,getCampoStr};

        void *campoIndexado = malloc(sizeof(void));
        int eh_campo_nulo;

        //obtenho o campo indexado (int ou str)
        campoIndexado = fncsGetCampo[tipDado](atual_reg, arq_index->campoIndexado);

        //vejo se o campo é nulo ou não
        eh_campo_nulo = fncsCampoNulo[tipDado](campoIndexado);

        if(eh_removido || eh_campo_nulo){
            //Se não é um registro indexável, tento ler o próximo
            *byteOffSetAnt = (*byteOffSetAnt) + consegui_ler;
            consegui_ler = ler_bin_registro(atual_reg, arq_dados->arqDados);
        }else{
            //Se consegui ler, devo indexá-lo
            liRegValido=1;

            //Crio o dado
            void *dadoIndexado;
            void *vetIndx;

            FncSetDadoIndx fncsSetDadoIndx[] = {setDadoIndxInt,setDadoIndxStr};
            FncSetVetIndx fncsSetVetIndx[] = {setVetIndx_int,setVetIndx_str};

            //escolho entre dado e o vetor para int ou str
            dadoIndexado = escolhe_indx_dado(arq_index);
            vetIndx = escolhe_vet_indx(arq_index);

            //escrevo as informações (campo indexado e byteOffSet)
            fncsSetDadoIndx[tipDado](dadoIndexado, *byteOffSetAnt, campoIndexado); 

            //agora escrevo o dado no vetor em RAM
            fncsSetVetIndx[tipDado](vetIndx, posReg, dadoIndexado);

            //agora incremento o contador de byteOffSet
            *byteOffSetAnt += consegui_ler;
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
    FncOrdemTipo fncs_ordena_vet_indx[] = {ordenaVetIndex_int,ordenaVetIndex_str};

    typedef void (*FncMostraTipo) (void*, int);
    FncMostraTipo fncs_MostraVet[] = {mostraVetInt,mostraVetStr}; 

    void *vet_indx = malloc(sizeof(void));
    vet_indx = escolhe_vet_indx(arq_index);

    // printf("antes de ordenar:\n");
    // fncs_MostraVet[arq_index->tipoDadoInt](vet_indx, qntd_reg);

    fncs_ordena_vet_indx[arq_index->tipoDadoInt](vet_indx, qntd_reg);

    // printf("depois de ordenar:\n");
    // fncs_MostraVet[arq_index->tipoDadoInt](vet_indx, qntd_reg);
}

void escreveArqIndex(ArqIndex_t *arq_index){

    escreveCabecalhoIndex(arq_index->arqIndex, arq_index->cabecalhoIndex);
    printf("cabecalho qtdReg=%d\n",get_qtdReg(arq_index->cabecalhoIndex));

    escreveVetIndex2(arq_index, 0, get_qtdReg(arq_index->cabecalhoIndex)-1);
}

void escreveVetIndex(ArqIndex_t *arq_index, int inicio, int fim){
    //Posiciono no local de escrita correto
    typedef int (*FncGetTamDadoIndx) (void);
    FncGetTamDadoIndx fncsGetTamDadoIndx[] = {getTamDadoIndx_int, getTamDadoIndx_str};

    int tamCabecalho = getTamCabecalhoIndx();
    int tamDado = fncsGetTamDadoIndx[arq_index->tipoDadoInt]();

    fseek(arq_index->arqIndex,tamCabecalho + (inicio*tamDado),SEEK_SET);

    //escolho que tipo de dado será escrito
    void *vet_indx = malloc(sizeof(void));
    vet_indx = escolhe_vet_indx(arq_index);
    typedef void (*FncEscreveIndx) (FILE*, void*, int);
    FncEscreveIndx fncsEscreveIndx[] = {escreveVetIndx_int,escreveVetIndx_str};

    //escrevo efetivamente o dado
    for(int cont = inicio; cont <= fim; ++cont){
        fncsEscreveIndx[arq_index->tipoDadoInt](arq_index->arqIndex, vet_indx, cont);    
    }
}

void escreveVetIndex2(ArqIndex_t *arq_index, int inicio, int fim){
    //escolho que tipo de dado será escrito
    void *vet_indx = escolhe_vet_indx(arq_index);

    typedef void (*FncEscreveIndx) (FILE*, void*, int);
    FncEscreveIndx fncsEscreveIndx[] = {escreveVetIndx_int,escreveVetIndx_str};

    int tipo_dado = arq_index->tipoDadoInt;//0 para int, 1 para string

    //escrevo efetivamente o dado
    for(int cont = inicio; cont <= fim; ++cont){

        fncsEscreveIndx[tipo_dado](arq_index->arqIndex, vet_indx, cont);    
    }
}


void terminaEscritaIndex(ArqIndex_t *arq_index, int qtndReg){
    //volto pára o início do arquivo e escrevo o cabeçalho final
    // printf("vou escrever %d registros\n", qtndReg);

    setCabecalhoIndex(
        arq_index->cabecalhoIndex,
        '1',
        qtndReg
    );

    fseek(arq_index->arqIndex, 0, SEEK_SET);
    escreveCabecalhoIndex(arq_index->arqIndex, arq_index->cabecalhoIndex);
}

void terminaEscritaDados(ArqDados_t *arq_dados, int qntdReg){
    setCabecalhoDados_nroRegArq(arq_dados->cabecalhoDados, qntdReg);
    // mostrar_cabecalho_dados(arq_dados->cabecalhoDados);
    rewind(arq_dados->arqDados);
    escrever_bin_registro_cabecalho(arq_dados->cabecalhoDados, arq_dados->arqDados);
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

InfoBusca_t *alocar_InfoBusca(int qtd_crit){
    InfoBusca_t *informacao = malloc(sizeof(InfoBusca_t));
    informacao->nomes = alocar_vetor_string(qtd_crit,TAM_MAX_NOME);
    informacao->vals_str = alocar_vetor_string(qtd_crit, TAM_MAX_VALOR);
    informacao->vals_int = malloc(sizeof(int)*qtd_crit);
    informacao->qtd_crit = qtd_crit;

    return informacao;
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

    //ler o buffer até a próxima linha
    ignorar_resto_linha();

    return criterios;
}

void desalocar_InfoBusca(InfoBusca_t *informacao){
    desalocar_vetor_string(informacao->nomes,informacao->qtd_crit);
    desalocar_vetor_string(informacao->vals_str,informacao->qtd_crit);
    free(informacao->vals_int);
    free(informacao);
}

void achouReg(void *arq_index, int flag){
    if(flag == 0){
        printf("Registro inexistente.\n");
    }
}

void nada(void *ponteiro, int flag){
    //função que não faz nada. Existe para manter a constância da função 'processaRegistros()', 
    //dado que nem sempre o parâmetro 'FncFinaliza final' é usado
}

int testarStatusDados(ArqDados_t *arq_dados){
	//funcao que retorna 1 caso o arquivo esteja consistente e 0 caso esteja inconsistente
	if(getStatusDados(arq_dados->cabecalhoDados) == '1'){
		return 1;
	}
	return 0;
}

int testarStatusIndex(ArqIndex_t *arq_index){
	//funcao que retorna 1 caso o arquivo esteja consistente e 0 caso esteja inconsistente
	if(getStatusIndex(arq_index->cabecalhoIndex) == '1'){
		return 1;
	}
	return 0;
}

void printa_busca(ArqDados_t *arq_dados, ArqIndex_t *arq_index, dados_t *registro, InfoBusca_t *alteracoes,long int byteOffSet){
    //funcao que ignora os campos do tipo void para que toda as funções de 
    //'acao' em 'processaRegistros()' sejam do mesmo tipo, no caso o 'FncAcao'

    //Em seguida, printa os campos de um registro
    mostrar_campos(registro);
}

void processaRegistros(ArqDados_t *arq_dados, ArqIndex_t *arq_index, InfoBusca_t *criterios, InfoBusca_t *alteracoes, FncAcao acao, FncFinaliza final){
    //funcao que define se a busca sera binaria no arquivo de indice ou sequencial no arquivo de dados

    //Alocar um vetor temporário, o qual irei editar ao longo do processo

    void *vetOrigem = escolhe_vet_indx(arq_index);
    int qtdRegOrigem = get_qtdReg(arq_index->cabecalhoIndex);

    if(strcmp(arq_index->tipoDado, "inteiro")==0){
        arq_index->vetTemp = (void*)aloc_vet_indx_DadoInt(get_qtdReg(arq_index->cabecalhoIndex));
        copiaVetIntparaVetTemp(arq_index->vetTemp,vetOrigem, qtdRegOrigem);
    }else if(strcmp(arq_index->tipoDado, "string")==0){
        arq_index->vetTemp = (void*)aloc_vet_indx_DadoStr(get_qtdReg(arq_index->cabecalhoIndex));
        copiaVetStrparaVetTemp(arq_index->vetTemp,vetOrigem, qtdRegOrigem);
    }

    arq_index->qtdReg_vetTemp = qtdRegOrigem;

    int existe = existe_index(criterios,arq_index);
    /*Se existe arquivo de indice para um dos campos a serem buscados,
    a variável 'existe' recebe o indice do nome desse campo no vet_nomes.
    Se não existe, a variável recebe -1.*/

    if(existe >= 0 ){
        /*se existe arquivo de index para um dos campos que se deseja 
        buscar, faz-se busca binária no arquivo de indice*/
        busca_bin_index(arq_index,arq_dados,existe,criterios,alteracoes,acao,final);
    }else{
        //se não, faz-se busca sequencial no arquivo de dados
    
        //Para isso, deve-se reiniciar o ponteiro do arquivo de dados para o primeiro registro de dados (pulando o cabecalho) 
        //para fazer um novo processamento, pois não há garantia de que o ponteiro esteja corretamente posicionado
        fseek(arq_dados->arqDados,len_cabecalho_dados(),SEEK_SET);

        //em seguida, chama-se a função que realiza a busca sequencial
        busca_seq_dados(arq_dados, arq_index, criterios,alteracoes,acao,final);
    }


    //INSERIR DESALOCARMENTO pija
}

void busca_bin_index(ArqIndex_t *arq_index, ArqDados_t *arq_dados, int pos_chave, InfoBusca_t *criterios, InfoBusca_t *alteracoes,FncAcao acao, FncFinaliza final){

    int tipoDado = arq_index->tipoDadoInt;

    typedef int (*FncBuscaBin) (void*, cabecalho_indx_t*, void*, int*);
    FncBuscaBin fncsBuscaBin[] = {busca_bin_int, busca_bin_str};

    FncGetByteOffSet fncsGetByteOffSet[] = {get_byteOffset_int, get_byteOffset_str};

    void *vetorIndex = escolhe_vet_indx(arq_index);
    void *chave = escolhe_criterio_vet_vals(criterios, pos_chave, tipoDado);

    int qtd_reg_val=0;//guarda o numero de registros que satisfazem o criterio de busca do arquivo de indice
    int pos_prim = fncsBuscaBin[tipoDado](vetorIndex, arq_index->cabecalhoIndex, chave, &qtd_reg_val);
    //fncsBuscaBin retorna o primeiro registro que satisfaz o critério de busca binária no vetorIndex.
    //Caso nenhum satisfaça, retorna -1. Além disso, motifica a variável "qtd_reg_val", por
    //referência, para o número de resgistros que satisfazem o critério de busca.
    //Dessa forma, o intervalo por_prim até [pos_prim+qtd_reg_val-1] é constituído por
    //todos os registros que satisfazem o critério de busca.

    //Com as informações sobre o intervalo (no vetIndex) que satisfaz os critérios de busca,
    //percorro o vetIndex.
    percorrer_index(fncsGetByteOffSet[tipoDado], pos_prim, qtd_reg_val, arq_dados, arq_index, criterios, alteracoes,acao, final);
}

void percorrer_index(FncGetByteOffSet get_byteOffset, int pos_prim, int qtd_reg_val, 
                    ArqDados_t *arq_dados, ArqIndex_t *arq_index,InfoBusca_t *criterios, InfoBusca_t *alteracoes,FncAcao acao, FncFinaliza final){

    printf("entrei percorrer_index com a pos:%d\n", pos_prim);
    
    int achei_reg_val = 0;
    //Flag que indica se algum registro satisafaz todos os critérios de busca.
    //Recebe -1 se nenhum satisfaz ou 1 se pelo menos 1 satisfaz

    if(pos_prim != -1){
        /*Como existe pelo menos 1 registro que satisfaz a busca, percorro o vet_indx_int para todos os 
        registros que satisfazem o criterio de busca do campo indexado e testo os outros criterios de busca*/

        printf("Antes do for do percorrer_index:qtd_reg_val:%d|qtdReg:%d\n",qtd_reg_val, get_qtdReg(arq_index->cabecalhoIndex));
        for(int i=0; i<qtd_reg_val; i++){
            printf("estou testando a posição:%d\n", pos_prim+i);
            //com a pos do primeiro e a qtd de registros, eu pego o byteoffset de todos eles

            void *vetor_real = escolhe_vet_indx(arq_index);
            long int byteOffset = get_byteOffset(vetor_real,pos_prim+i);
            
            //pra cada byteoffset, eu checo todos os criterios
            dados_t *reg = alocar_dados();
            getRegistro(byteOffset, arq_dados->arqDados, reg);
            
            mostrar_campos(reg);
            
            int satisfaz = testar_criterios(reg, criterios->nomes, criterios->vals_str, criterios->vals_int, criterios->qtd_crit);

            if(satisfaz){
                //se o reg satisfaz todos os criterios, realizo a ação
                printf("posicao %d satisfaz (byte:%ld)\n", pos_prim + i, byteOffset);
                acao(arq_dados, arq_index, reg, alteracoes,byteOffset);
                achei_reg_val = 1;
            }else{
                printf("posicao %d não satisfaz (byte:%ld)\n", pos_prim + i, byteOffset);
            }

            desalocar_registro(reg);
        }
    }

    if(achei_reg_val != 0){
        achei_reg_val = get_nroRegIndex(arq_index);
    }

    final(arq_index, achei_reg_val);
}

void busca_seq_dados(ArqDados_t *arq_dados, ArqIndex_t *arq_index,InfoBusca_t *criterios, InfoBusca_t *alteracoes,FncAcao acao, FncFinaliza final){
    
    int achei_reg_val = 0;
    //Flag que indica se algum registro satisafaz todos os critérios de busca.
    //Recebe -1 se nenhum satisfaz ou 1 se pelo menos 1 satisfaz

    dados_t *registro = alocar_dados();//aloco um memoria para leitura de um registro de dados
        
    // typedef void (*FncMostraTipo) (void*, int);
    // FncMostraTipo fncs_MostraVet[] = {mostraVetInt,mostraVetStr}; 
    // printf("vetIndxTemp na busca_seq_dados:\n");
    // fncs_MostraVet[arq_index->tipoDadoInt](arq_index->vetTemp, arq_index->qtdReg_vetTemp);


    long int byteOffSet_atual = len_cabecalho_dados();

    //Consegui_ler armazena -1 se não conseguiu ler e armazena o tamanho do registro caso consiga ler
    int consegui_ler = ler_bin_registro(registro, arq_dados->arqDados);

    while(consegui_ler>0){
        //Se consegui ler, avalio os criterios de busca

        if(testar_criterios(registro,criterios->nomes,criterios->vals_str,criterios->vals_int,criterios->qtd_crit)){
            printf("satisfaz ");
            mostrar_campos(registro);
            //se o registro satisfaz todos os criterios, realizo a ação 
            achei_reg_val = 1;//achei pelo menos 1 registro que satisfaz os critérios
            acao(arq_dados, arq_index, registro, alteracoes, byteOffSet_atual);
        }

        byteOffSet_atual += consegui_ler;
        //sempre desaloco o registro, pois preciso desalocar os campos de tamanho variavel do registro
        desalocar_registro(registro);
        //Em seguida aloco outro registro, para fazer a leitura
        registro = alocar_dados();
        consegui_ler = ler_bin_registro(registro, arq_dados->arqDados);
    }
    //se não conseguiu ler, os campos de tamanho variavel nao foram alocados. Assim, desaloco apenas o ponteiro para o tipo dados_t

    printf("vou realizar ação final\n");
    free(registro);
    
    if(achei_reg_val != 0){
        achei_reg_val = get_nroRegIndex(arq_index);
    }
    final(arq_index, achei_reg_val);
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

void ordenaVetIndexFinal(void *arq_index, int qtdReg_ordenar){
    printf("cheguei no final. Campo qtdReg_ordenar:%d\n", qtdReg_ordenar);
    if(qtdReg_ordenar > 0){
        //Tenho que passar o vetor temporário para o original, para isso:
        //1-faço o casting do arq_index
        ArqIndex_t *arq_index_real = (ArqIndex_t*)arq_index;
        //2-realoco o vetor original
        int qtdReg_antes = get_qtdReg(arq_index_real->cabecalhoIndex);
        int qtdReg_depois = arq_index_real->qtdReg_vetTemp;
        int acrescimo = qtdReg_depois - qtdReg_antes;
        realocar_vet_index(arq_index_real, qtdReg_antes, acrescimo);
        //3-modifico a qtdReg original
        set_qtdReg(arq_index_real->cabecalhoIndex, qtdReg_depois);
        //Agora, ordeno o novo
        ordenaVetIndex(arq_index_real, qtdReg_ordenar);
        printf("terminei a ordenação\n");
    }
}

int inserirRegStdin(ArqDados_t *arq_dados, ArqIndex_t *arq_index, int pos){
    /*
            Lê um buffer da entrada padrão stdin e insere o
        Respectivo registro no arquivo de dados e de index.
    */

    dados_t *reg_inserir = alocar_dados();

    leRegStdin(reg_inserir);

    inserirReg(arq_dados, arq_index, reg_inserir, pos);
}

void inserirReg(ArqDados_t *arq_dados, ArqIndex_t *arq_index, dados_t *reg, int pos){
    int tipDado = arq_index->tipoDadoInt;

    FncCampoNulo fncsCampoNulo[] = {campoNulo_int,campoNulo_str};
    FncGetCampoIndexIndexado fncsGetCampo[] = {getCampoInt,getCampoStr};

    void *campoIndexado = malloc(sizeof(void));
    int eh_campo_nulo;
    
    //obtenho o campo indexado (int ou str)
    campoIndexado = fncsGetCampo[tipDado](reg, arq_index->campoIndexado);

    //Vejo se o campo é nulo ou não (para poder indexá-lo)
    eh_campo_nulo = fncsCampoNulo[tipDado](campoIndexado);

    if(eh_campo_nulo == 0){//Se não é campo nulo
        int antigoQtdReg = get_nroRegIndex(arq_index);
        setCabecalhoIndex(arq_index->cabecalhoIndex, '0', ++antigoQtdReg);

        long int byte_reg_inserir;
        byte_reg_inserir = get_proxByteOffset(arq_dados->cabecalhoDados);

        //Com o campo indexado e com o byteOffSet, consigo criar dado
        //de index do registro a ser inserido

        void *dadoInserir = malloc(sizeof(void));
        void *vetIndex = malloc(sizeof(void));

        FncSetDadoIndx fncsSetDadoIndx[] = {setDadoIndxInt,setDadoIndxStr};
        FncSetVetIndx fncsSetVetIndx[] = {setVetIndx_int,setVetIndx_str};

        dadoInserir = escolhe_indx_dado(arq_index);
        vetIndex = escolhe_vet_indx(arq_index);

        fncsSetDadoIndx[tipDado](dadoInserir, byte_reg_inserir, campoIndexado);
        fncsSetVetIndx[tipDado](vetIndex, pos, dadoInserir);

        setCabecalhoIndex(arq_index->cabecalhoIndex, '0', pos+1);
    }

    //Escrevo no registro de dados
    prepara_para_escrita(reg);
    escrever_bin_registro_dados(reg,arq_dados->arqDados,arq_dados->cabecalhoDados); 
    //incremento o nroRegArq
    cabecalho_nroRegArq_incrementar(arq_dados->cabecalhoDados, 1);
}

void inserirRegTemp(ArqDados_t *arq_dados, ArqIndex_t *arq_index, dados_t *reg, int pos){
    int tipDado = arq_index->tipoDadoInt;

    FncCampoNulo fncsCampoNulo[] = {campoNulo_int,campoNulo_str};
    FncGetCampoIndexIndexado fncsGetCampo[] = {getCampoInt,getCampoStr};

    void *campoIndexado = malloc(sizeof(void));
    int eh_campo_nulo;
    
    //obtenho o campo indexado (int ou str)
    campoIndexado = fncsGetCampo[tipDado](reg, arq_index->campoIndexado);

    //Vejo se o campo é nulo ou não (para poder indexá-lo)
    eh_campo_nulo = fncsCampoNulo[tipDado](campoIndexado);

    if(eh_campo_nulo == 0){//Se não é campo nulo
        arq_index->qtdReg_vetTemp++;

        long int byte_reg_inserir;
        byte_reg_inserir = get_proxByteOffset(arq_dados->cabecalhoDados);

        //Com o campo indexado e com o byteOffSet, consigo criar dado
        //de index do registro a ser inserido

        void *dadoInserir = malloc(sizeof(void));

        FncSetDadoIndx fncsSetDadoIndx[] = {setDadoIndxInt,setDadoIndxStr};
        FncSetVetIndx fncsSetVetIndx[] = {setVetIndx_int,setVetIndx_str};

        dadoInserir = escolhe_indx_dado(arq_index);

        fncsSetDadoIndx[tipDado](dadoInserir, byte_reg_inserir, campoIndexado);
        fncsSetVetIndx[tipDado](arq_index->vetTemp, pos, dadoInserir);
    }

    //Escrevo no registro de dados
    prepara_para_escrita(reg);
    escrever_bin_registro_dados(reg,arq_dados->arqDados,arq_dados->cabecalhoDados); 
    //incremento o nroRegArq
    cabecalho_nroRegArq_incrementar(arq_dados->cabecalhoDados, 1);
}

int modificaCampoIndexado(ArqIndex_t *arq_index, InfoBusca_t *altera){
    //Retorna 1 se o campoIndexado é alterado, 0 caso contrário
    for(int i = 0; i < altera->qtd_crit; ++i){
        if(strcmp(arq_index->tipoDado, altera->nomes[i])==0){
            return 1;
        }
    }
    return 0;
}

int obterPosicaoRegVetIndx(ArqDados_t *arq_dados, ArqIndex_t *arq_index, dados_t *reg_atual, long int byteOffSet){
    //Obter a posição do registro dentro do vetIndex a partir do byteOffSet e do campo indexado.

    int tipoDado = arq_index->tipoDadoInt;

    void *dadoIndex = escolhe_indx_dado(arq_index);
    
    FncGetCampoIndexIndexado fncsGetCampo[] = {getCampoInt, getCampoStr};
    void *chave = fncsGetCampo[tipoDado](reg_atual, arq_index->campoIndexado);

    FncSetDadoIndx fncsSetDadoIndx[] = {setDadoIndxInt, setDadoIndxStr};
    fncsSetDadoIndx[tipoDado](dadoIndex, byteOffSet, chave);

    printf("dado que chegou no obter posicao\n");
    mostraRegIndx_str(dadoIndex);

    FncComparacao fncsComparacao[] = {comparacao_vet_dados_indx_int_RegIndx, comparacao_vet_dados_indx_str_RegIndx};
    int pos = busca_bin_rec(arq_index->vetTemp, 0, arq_index->qtdReg_vetTemp, dadoIndex, fncsComparacao[tipoDado]);

    return pos;
}

void modificaReg(ArqDados_t *arq_dados, ArqIndex_t *arq_index, dados_t *reg_atual, InfoBusca_t *alteracoes,long int byteOffSet){

    dados_t *reg_modificado = alocar_dados();
    copia_registro(reg_modificado, reg_atual);
    fazAlteracoes(reg_modificado, alteracoes->nomes, alteracoes->vals_str, alteracoes->vals_int, alteracoes->qtd_crit);//abobra

    int campo_modificado_eh_nulo;
    int qtd_reg = arq_index->qtdReg_vetTemp;
    int tipoDado = arq_index->tipoDadoInt;
    int tam_reg_atual = len_reg_dados(reg_atual);
    int tam_reg_modificado = len_reg_dados(reg_modificado);
    int pos = obterPosicaoRegVetIndx(arq_dados, arq_index, reg_atual, byteOffSet);
    // void *vetIndx = escolhe_vet_indx(arq_index);

    //Devo ver se o campo indexado é nulo. Para isso:
    void *campoIndexado;
    FncGetCampoIndexIndexado fncsGetCampoIndexado[] = {getCampoInt,getCampoStr};
    FncCampoNulo fncsCampoNulo[] = {campoNulo_int, campoNulo_str};
    //1-pego o novo campoIndexado (que foi modificado)
    campoIndexado = fncsGetCampoIndexado[tipoDado](reg_modificado, arq_index->campoIndexado);
    //2-vejo se esse campo é nulo
    campo_modificado_eh_nulo = fncsCampoNulo[tipoDado](campoIndexado);
    
    //flag que avisa se modificou o campo indexado
    int modifica_campo_indexado = modificaCampoIndexado(arq_index, alteracoes);
    
    //Devo voltar o cursor do arqDados para o começo do 
    //registro que acabei de ler.
    fseek(arq_dados->arqDados, (-1)*tam_reg_atual, SEEK_CUR);

    printf("cheguei na acao: pos:%d|qtdRegVetTemp:%d\n", pos, arq_index->qtdReg_vetTemp);

    printf("acao para um registro que:");
    if(tam_reg_atual < tam_reg_modificado){
        printf("cabe ,");
        //Se cabe
        //Escrevo na posição atual
        reescrever_registro_dados(reg_modificado, arq_dados->arqDados);
        completaRegistroComDollar(arq_dados->arqDados, tam_reg_atual - tam_reg_modificado);
        
        if(modifica_campo_indexado){
            //Se modifica o campo indexado
            if(pos != -1){
                //Se o registro tinha campo indexado
                //Removo do vetIndex
                desindexaRegistro(arq_index, pos);
            }

            if(campo_modificado_eh_nulo == 0){
                //Reescrevo o indexamento no vetIndex

                //a qtd_reg pode ter mudado, pois posso ter desidexado o registro, então
                qtd_reg = arq_index->qtdReg_vetTemp;
                
                realocar_vetIndxTemp(arq_index, qtd_reg, 1);
                arq_index->qtdReg_vetTemp++;

                void *dadoIndx = escolhe_indx_dado(arq_index);
                FncSetVetIndx fncsSetVetIndx[] = {setVetIndx_int,setVetIndx_str};
                FncSetDadoIndx fncsSetDadoIndx[] = {setDadoIndxInt,setDadoIndxStr};

                printf("na ação (quando cabe), vou inserir o que chegou na pos=%d na pos=%d\n", pos, qtd_reg);
                fncsSetDadoIndx[tipoDado](dadoIndx, byteOffSet, campoIndexado);//configuro o novo dado
                fncsSetVetIndx[tipoDado](arq_index->vetTemp, qtd_reg, dadoIndx);//coloco na última posição
            }
        }

    }else{
        //Se não cabe
        printf("não cabe, ");
        if(pos != -1){
            printf("tinha campo indexado, ");
            //Se o registro tinha campo indexado
            //Removo do vetIndex
            desindexaRegistro(arq_index, pos);
        }

        //marco como removido
        escreverCampoRemovido(arq_dados->arqDados);
        //incremento o nro_RegRem
        setCabecalhoDados_nroRegRem(arq_dados->cabecalhoDados, get_nroRegRem(arq_dados->cabecalhoDados)+1);
        
        //Escrevo no final. Para isso:
        //1- realoco se o campoIndexado não for nulo
        if(campo_modificado_eh_nulo == 0){//campo modificado não é nulo
            //incremento o número de registros
            printf("campo modificado não é nulo\n");
            qtd_reg = arq_index->qtdReg_vetTemp;
            arq_index->qtdReg_vetTemp++;
            realocar_vetIndxTemp(arq_index, qtd_reg, 1);
        }
        //2- vou para o final do arquivo de dados e faço o append da função 6
        fseek(arq_dados->arqDados, 0, SEEK_END);

        // qtd_reg = arq_index->qtdReg_vetTemp;
        printf("na ação (quando não cabe), vou inserir o que chegou na pos=%d na pos=%d\n", pos, qtd_reg);
        inserirReg(arq_dados, arq_index, reg_modificado, qtd_reg);
    }
}

void editarRegStdin(ArqIndex_t *arq_index, ArqDados_t *arq_dados){

    //ler o critérios de busca
    InfoBusca_t *criterios = ler_criterios_busca();
    InfoBusca_t *alteracoes = ler_criterios_busca();

    processaRegistros(arq_dados,arq_index,criterios,alteracoes,modificaReg,ordenaVetIndexFinal);

    desalocar_InfoBusca(criterios);
    desalocar_InfoBusca(alteracoes);
}

void deletarRegistro(ArqDados_t *arq_dados, ArqIndex_t *arq_index, dados_t *registro, InfoBusca_t *ignorar, long int byteOffset){

    //faço a remoção lógica do registro
    remocaoLogica(registro,arq_dados->cabecalhoDados);

    //Agora devo reescrever o campo que foi atualizado no arquivo de dados. Para isso preciso:

    //-retornar à posição inicial do registro no arquivo de dados;
    fseek(arq_dados->arqDados,byteOffset,SEEK_SET);

    //-e escrever efetivamente
    escrever_campo_removido(arq_dados->arqDados ,registro);

    /*Agora, devo remover o registro do arquivo de índice, se necessário. Para removê-lo de lá,
    preciso de sua posição no vetor de dados do arquivo de índice. Como o 'deletarRegistro()' 
    recebe apenas o byteOffset desse registro no arquivo de dados, devo buscar a posição dele 
    no vetor de dados do arquivo de índice.*/
    int pos = obterPosicaoRegVetIndx(arq_dados, arq_index, registro, byteOffset);

    if(pos != -1){
        //Se a posição é válida, significa que o registro está indexado. 
        //Assim, removo o registro do arquivo de índice a partir de sua posição no vetor de dados
        desindexaRegistro(arq_index,pos);
    }
    //se é inválida, o registro não está indexado, então não preciso fazer nada
}

void desindexaRegistro(ArqIndex_t *arq_index, int pos){
    //função que remove um registro do arquivo de índice por meio de informações temporárias.
    
    //decremento a quantidade de registros indexados no vetor temporário;
    int qtdReg_ant = arq_index->qtdReg_vetTemp;//quantidade de registros antes de remover 1
    arq_index->qtdReg_vetTemp = qtdReg_ant-1;//quantidade agora que removi 1

    //Agora quero fazer o shift das posições do vetor de dados, a partir da pos que se deseja remover
    //Para isso:

    //-defino um vetor de funções que fazem o shift para tipos específicos
    typedef void (*FncShiftar)(void *,int, int);
    FncShiftar fncsShiftar[] = {shiftarVetIndxInt,shiftarVetIndxStr};

    //-descubro qual é o tipo do vetor de dados que vou usar (0 é inteiro e 1 é string)
    int tipo_dado = arq_index->tipoDadoInt;

    //-por fim, chamo a função que faz o shift 
    fncsShiftar[tipo_dado](arq_index->vetTemp,pos,qtdReg_ant-1);

    //-em seguida, realoco o tamanho do vetor
    realocar_vetIndxTemp(arq_index, qtdReg_ant, (-1));
}

void reiniciarCursorIndex(ArqIndex_t *arq_index){
    fseek(arq_index->arqIndex,0,SEEK_SET);
}

void reiniciarCursorDados(ArqDados_t *arq_dados){
    fseek(arq_dados->arqDados,0,SEEK_SET);
}

void alterarStatusIndex(ArqIndex_t *arq_index, int status){
    if(status == 1){
        setStatusIndex(arq_index->cabecalhoIndex,'1');
    }else if(status == 0){
        setStatusIndex(arq_index->cabecalhoIndex,'0');
    }
}

void alterarStatusDados(ArqDados_t *arq_dados, int status){
    if(status == 1){
        setStatusDados(arq_dados->cabecalhoDados,'1');
    }else if(status == 0){
        setStatusDados(arq_dados->cabecalhoDados,'0');
    }
}

void escreverStatusDados(ArqDados_t *arq_dados){
    fwriteStatusDados(arq_dados->arqDados, arq_dados->cabecalhoDados);
}

void escreverStatusIndex(ArqIndex_t *arq_index){
    fwriteStatusIndex(arq_index->arqIndex, arq_index->cabecalhoIndex);
}