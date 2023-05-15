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
    if(testarStatusDados(arq_dados)==0){
        mensagem_erro();
    }

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

    //Testo se os dois arquivos estao consistentes. Se não estão, encerro o programa com uma mensagem de erro.
    if(testarStatusIndex(arq_index)==0){
        mensagem_erro();
    }else if(testarStatusDados(arq_dados)==0){
        mensagem_erro();
    }

    //Loop que faz n buscas
    for(int i=1; i<=n; i++){
        printf("Resposta para a busca %d\n",i);

        //Ler os critérios de busca
        InfoBusca_t *criterios = ler_criterios_busca();
        InfoBusca_t *alteracoes;
        /*Processar o registro usando a ação 'printa_busca'
        e o final 'achouReg', que diz se o registro é inexistente, 
        caso nenhum satisfaça os critérios de busca*/
        
        processaRegistros(arq_dados,arq_index,criterios,alteracoes,printa_busca,achouReg);

        //Desalocar crtérios de busca    	
        desalocar_InfoBusca(criterios);
    }

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
    abrir_arq_dados(arq_dados, "r+b");
    abrir_arq_index(arq_index, "r+b");

    //Carrego o arquivo de indice na memoria primaria
    ler_cabecalho_arq_index(arq_index);
    ler_dados_arq_index(arq_index);

    //Ler o cabeçalho do arquivo de dados
    ler_cabecalho_dados(arq_dados);

    //Testo se os dois arquivos estao consistentes. Se não estão, encerro o programa com uma mensagem de erro.
    /* if(testarStatusIndex(arq_index)==0){
        mensagem_erro();
    }else if(testarStatusDados(arq_dados)==0){
        mensagem_erro();
    } */

    //indico que os arquivos estão inconsistentes, pois vou escrever em ambos
    //como li todo o arquivo de index, o cursor está no final
    reiniciarCursorIndex(arq_index);
    alterarStatusIndex(arq_index,0);
    escreverStatusIndex(arq_index);

    //como li o cabecalho do arquivo de dados, o cursor está logo após o cabecalho
    reiniciarCursorDados(arq_dados);
    alterarStatusDados(arq_dados,0);
    escreverStatusDados(arq_dados);

    //Loop que faz 'n' deleções
    for(int i=1; i<=n; i++){

        //Ler os critérios de busca para os registros que se deseja remover
        InfoBusca_t *criterios = ler_criterios_busca();
        InfoBusca_t *alteracoes;

        //Processar o registro usando a ação 'deletar_registros' e o final 'nada'
        processaRegistros(arq_dados,arq_index,criterios,alteracoes,deletarRegistro, nada);

        //Desalocar tipos utilizados    	
        desalocar_InfoBusca(criterios);
    }

    //REESCREVO TODO O ARQUIVO DE INDEX
    reiniciarCursorIndex(arq_index);
    escreveArqIndex(arq_index);

    //Indico que os arquivos estão consistentes, pois já usei ambos
    //como escrevi todo o arquivo de index, o cursor está no final
    reiniciarCursorIndex(arq_index);
    alterarStatusIndex(arq_index,1);
    escreverStatusIndex(arq_index);

    //Como o 'processaRegistros()' pode alterar o cursor do arquivo de dados, reinicio ele
    reiniciarCursorDados(arq_dados);
    alterarStatusDados(arq_dados,1);
    escreverStatusDados(arq_dados);

    //Chama a funcao binarioNaTela() para gerar uma saída com base nas mudanças nos arquivos
    binarioNaTela(getNomeArqDados(arq_dados)); 
    binarioNaTela(getNomeArqIndex(arq_index));
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
    abrir_arq_index(arq_index, "r+b");
    abrir_arq_dados(arq_dados, "r+b");

    //Ler os cabeçalhos
    ler_cabecalho_arq_index(arq_index);
    ler_dados_arq_index(arq_index);
    ler_cabecalho_dados(arq_dados);

    levaFinalCursorDados(arq_dados);

    int qtdInserir;
    int qtdRegEscritosIndex;
    int qtdRegIndexAntes;
    int qtdRegDadosAntes;
    int qtdRegEscritosDados;

    scanf(" %d", &qtdInserir);

    qtdRegIndexAntes = get_nroRegIndex(arq_index);
    qtdRegDadosAntes = get_nroRegValidos(arq_dados);
    qtdRegEscritosDados = qtdRegDadosAntes + qtdInserir;

    realocar_vet_index(arq_index, qtdRegIndexAntes, qtdInserir);

    int regNaoInseridos = 0;
    int pos = qtdRegDadosAntes;
    for(int cont = 0; cont < qtdInserir; ++cont){
        // printf("cont:%d\n", cont);
        inserirRegStdin(arq_dados,arq_index,pos);
        pos = get_nroRegIndex(arq_index);
    }

    qtdRegEscritosIndex = get_nroRegIndex(arq_index);

    ordenaVetIndex(arq_index, qtdRegEscritosIndex);

    fechar_arq_index(arq_index);
    abrir_arq_index(arq_index, "wb");

    escreveVetIndex(arq_index, 0, qtdRegEscritosIndex-1);
    terminaEscritaIndex(arq_index, qtdRegEscritosIndex);
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
    scanf("%d\n",&n);

    //Com os inputs armazenados, faço a abertura dos arquivos.
    abrir_arq_dados(arq_dados, "r+b");
    abrir_arq_index(arq_index, "r+b");

    //Carrego o arquivo de indice na memoria primaria.
    ler_cabecalho_arq_index(arq_index);
    ler_dados_arq_index(arq_index);

    //Ler o cabeçalho do arquivo de dados.
    ler_cabecalho_dados(arq_dados);

    int status_ok = 1;
    status_ok *= testarStatusDados(arq_dados);
    status_ok *= testarStatusIndex(arq_index);
    if(status_ok == 0){
        mensagem_erro();
    }else{
        //Se o status está ok, modifico para fazer alterações
        alterarStatusDados(arq_dados, '0');
        alterarStatusIndex(arq_index, '0');
    }

    int cont_n;
    for(cont_n = 0; cont_n < n; cont_n++){
        printf("busca %d\n", cont_n);
        editarRegStdin(arq_index,arq_dados);
    }

    //Total de registros depois das modificações.
    int qtdRegIndexDepois = get_nroRegIndex(arq_index);
    int qtdRegDadosDepois = get_nroRegValidos(arq_dados);

    escreveVetIndex(arq_index, 0, qtdRegDadosDepois-1);

    terminaEscritaDados(arq_dados, qtdRegDadosDepois);
    terminaEscritaIndex(arq_index, qtdRegIndexDepois);

    binarioNaTela(getNomeArqDados(arq_dados));
    binarioNaTela(getNomeArqIndex(arq_index));

    fechar_arq_dados(arq_dados);
    fechar_arq_index(arq_index);

    desalocar_ArqDados(arq_dados);
    desalocar_ArqIndex(arq_index);
}