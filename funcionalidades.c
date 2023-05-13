#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "auxiliares.h"
#include "manipulacao.h"

//debug
void debug(void){
    //nomeArqIndx.bin tipoDado

    ArqIndex_t *arq_index = alocar_arq_index();
    ler_nome_arq_index(arq_index);
    ler_tipoDado(arq_index);
    abrir_arq_index(arq_index, "rb");
    ler_cabecalho_arq_index(arq_index);
    ler_dados_arq_index(arq_index);

    int nroRegVal = get_nroRegIndex(arq_index);
    printf("existem %d registros indexados\n", nroRegVal);

    alocar_vet_index(arq_index, nroRegVal);
    mostrar_arq_index(arq_index);

    desalocar_ArqIndex(arq_index);
    fechar_arq_index(arq_index);
}

//Funcionalidade [3]
void create_index(){

    //Aloca os tipos que serão usados
    ArqDados_t *arq_dados = alocar_arq_dados();
    erro(arq_dados);
    ArqIndex_t *arq_index = alocar_arq_index();
    erro(arq_index);

    //Faz a leitura dos inputs
    ler_nome_arq_dados(arq_dados);
    ler_campoIndexado(arq_index);
    ler_tipoDado(arq_index);
    ler_nome_arq_index(arq_index);
    // printf("leitura dos dados concluída\n");
    

    //Com os inputs armazenados, faço 
    //a abertura dos arquivos.
    abrir_arq_dados(arq_dados, "rb");
    abrir_arq_index(arq_index, "wb");
    // printf("abertura dos arquivos concluída\n");

    //Ler o cabeçalho do arquivo de dados
    ler_cabecalho_dados(arq_dados);

    long int byteOffSetAtual;
    byteOffSetAtual = getTamCabecalhoDados(arq_dados);

    //Verificar consistência dos dados do arquivo
    confere_arq_dados(arq_dados);

    //Aloco espaço para o vetor que armazenará o arquivo de index
    unsigned int nroRegValidos = get_nroRegValidos(arq_dados);
    alocar_vet_index(arq_index, nroRegValidos);

    int foi_escrito;
    //tem 1 se conseguiu indexar o registro, 0 caso contrário

    int pos_reg = 0;
    //Guarda a posição que será escrito o registro  

    do{
        foi_escrito = indexaRegistro(
            arq_dados,
            arq_index,
            pos_reg,
            &byteOffSetAtual
        );
        pos_reg++;
    }while(foi_escrito==1);

    //Com o os index carregados em RAM, faço a ordenação
    int qntd_registros = pos_reg-1;
    ordenaVetIndex(arq_index, qntd_registros);

    //Com os dados ordenados, escrevo-os no arquivo de index
    escreveVetIndex(arq_index, 0, qntd_registros-1);
    terminaEscritaIndex(arq_index, qntd_registros);

    //Fechar arquivos
    fechar_arq_index(arq_index);
    fechar_arq_dados(arq_dados);

    binarioNaTela(getNomeArqIndex(arq_index));

    //Desalocar tipos utilizados
    desalocar_ArqDados(arq_dados);
    desalocar_ArqIndex(arq_index);

    // printf("fechamento dos arquivos concluído\n");
}

//Funcionalidade [4]
void where(void){
    //Aloca os tipos que serão usados
    ArqDados_t *arq_dados = alocar_arq_dados();
    erro(arq_dados);
    ArqIndex_t *arq_index = alocar_arq_index();
    erro(arq_index);
    int n;

    //Faz a leitura dos inputs
    ler_nome_arq_dados(arq_dados);
    ler_campoIndexado(arq_index);
    ler_tipoDado(arq_index);
    ler_nome_arq_index(arq_index);
    scanf("%d",&n);

    //Com os inputs armazenados, faço a abertura dos arquivos.
    abrir_arq_dados(arq_dados, "rb");
    abrir_arq_index(arq_index, "rb");

    //Carrego o arquivo de indice na memoria primaria
    ler_cabecalho_arq_index(arq_index);
    ler_dados_arq_index(arq_index);

    //Ler o cabeçalho do arquivo de dados
    ler_cabecalho_dados(arq_dados);

    //Testo se os dois arquivos estao consistentes
    if(testar_status(arq_index,arq_dados) != 2){
        mensagem_erro();
    }

    //Chamo a funcao que busca e printa os registros desejados
    busca(arq_dados,arq_index,n);

    //Fechar arquivos
    fechar_arq_index(arq_index);
    fechar_arq_dados(arq_dados);

    //Desalocar tipos utilizados
    desalocar_ArqDados(arq_dados);
    desalocar_ArqIndex(arq_index);
}

//Funcionalidade [5]
void delete_from(){
    //Aloca os tipos que serão usados
    ArqDados_t *arq_dados = alocar_arq_dados();
    erro(arq_dados);
    ArqIndex_t *arq_index = alocar_arq_index();
    erro(arq_index);
    int n;

    //Faz a leitura dos inputs
    ler_nome_arq_dados(arq_dados);
    ler_campoIndexado(arq_index);
    ler_tipoDado(arq_index);
    ler_nome_arq_index(arq_index);
    scanf("%d",&n);

    //Com os inputs armazenados, faço a abertura dos arquivos.
    abrir_arq_dados(arq_dados, "rb");
    abrir_arq_index(arq_index, "rb");

    //Carrego o arquivo de indice na memoria primaria
    ler_cabecalho_arq_index(arq_index);
    ler_dados_arq_index(arq_index);

    // deletar(arq_dados,arq_index,n);
}

//Funcionalidade [6]
void insert_into(){
    //Alocar tipos utilizados
    ArqDados_t *arq_dados = alocar_arq_dados();
    erro(arq_dados);
    ArqIndex_t *arq_index = alocar_arq_index();
    erro(arq_index);

    //Ler entrada de dados
    ler_nome_arq_dados(arq_dados);
    ler_campoIndexado(arq_index);
    ler_tipoDado(arq_index);
    ler_nome_arq_index(arq_index);

    //abrir arquivos
    abrir_arq_index(arq_index, "rb");
    abrir_arq_dados(arq_dados, "a+b");

    //Ler os cabeçalhos
    ler_cabecalho_arq_index(arq_index);
    ler_dados_arq_index(arq_index);
    ler_cabecalho_dados(arq_dados);

    int qtdInserir;
    int qtdRegEscritosIndex;
    int qtdRegIndexAntes;
    int qtdRegDadosAntes;
    int qtdRegEscritosDados;

    scanf(" %d", &qtdInserir);

    qtdRegIndexAntes = get_nroRegIndex(arq_index);
    qtdRegDadosAntes = get_nroRegValidos(arq_dados);

    realocar_vet_index(arq_index, qtdRegIndexAntes, qtdInserir);

    int regNaoInseridos = 0;
    for(int cont = 0; cont < qtdInserir; ++cont){
        // printf("cont:%d\n", cont);
        inserirRegStdin(
            arq_dados,
            arq_index,
            cont+qtdRegIndexAntes-regNaoInseridos
        );
    }

    qtdRegEscritosIndex = get_nroRegIndex(arq_index);

    ordenaVetIndex(arq_index, qtdRegEscritosIndex);

    fechar_arq_index(arq_index);
    abrir_arq_index(arq_index, "wb");

    escreveVetIndex(arq_index, 0, qtdRegEscritosIndex-1);
    terminaEscritaIndex(arq_index, qtdRegEscritosIndex);

    fechar_arq_dados(arq_dados);
    abrir_arq_dados(arq_dados, "r+b");

    terminaEscritaDados(arq_dados, qtdRegEscritosDados);

    //Fechar os arquivos utilizados
    fechar_arq_dados(arq_dados);
    fechar_arq_index(arq_index);

    binarioNaTela(getNomeArqDados(arq_dados)); 
    binarioNaTela(getNomeArqIndex(arq_index));

    //Desalocar os tipos utilizados
    desalocar_ArqDados(arq_dados);
    desalocar_ArqIndex(arq_index);
}

//Funcionalidade [7]
void update(){
    //Aloca os tipos que serão usados.
    ArqDados_t *arq_dados = alocar_arq_dados();
    erro(arq_dados);
    ArqIndex_t *arq_index = alocar_arq_index();
    erro(arq_index);
    int n;

    //Faz a leitura dos inputs.
    ler_nome_arq_dados(arq_dados);
    ler_campoIndexado(arq_index);
    ler_tipoDado(arq_index);
    ler_nome_arq_index(arq_index);
    scanf("%d\n\n",&n);

    //Com os inputs armazenados, faço a abertura dos arquivos.
    abrir_arq_dados(arq_dados, "r+b");
    abrir_arq_index(arq_index, "r+b");

    //Carrego o arquivo de indice na memoria primaria.
    ler_cabecalho_arq_index(arq_index);
    ler_dados_arq_index(arq_index);

    //Ler o cabeçalho do arquivo de dados.
    ler_cabecalho_dados(arq_dados);

    //Testo se os dois arquivos estao consistentes.
    if(testar_status(arq_index,arq_dados) != 2){
        mensagem_erro();
    }

    //Total de registros antes das modificações.
    int qtdRegIndexAntes = get_nroRegIndex(arq_index);
    int qtdRegDadosAntes = get_nroRegValidos(arq_dados);

    int cont_n;
    for(cont_n = 0; cont_n < n; cont_n++){
        editarRegStdin(
            arq_index,
            arq_dados,
            &qtdRegDadosAntes,
            &qtdRegIndexAntes
        );
    }
}