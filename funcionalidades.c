#include <stdio.h>
#include <stdlib.h>
#include "auxiliares.h"
#include "manipulacao.h"

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
    printf("leitura dos dados concluída\n");

    //Com os inputs armazenados, faço a abertura dos arquivos.
    abrir_arq_dados(arq_dados, "rb");
    abrir_arq_index(arq_index, "wb");
    printf("abertura dos arquivos concluída\n");

    //Ler o cabeçalho do arquivo de dados
    ler_cabecalho_dados(arq_dados);

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
        foi_escrito = indexaRegistro(arq_dados, arq_index, pos_reg);
        pos_reg++;
    }while(foi_escrito==1);

    //Com o os index carregados em RAM, faço a ordenação
    int qntd_registros = pos_reg-1;
    ordenaVetIndex(arq_index, qntd_registros);

    //Com os dados ordenados, escrevo-os no arquivo de index
        //escreveVetIndex(arq_index, 0, qntd_registros-1);

    /*
    -----------------------------------
    TA FALTANDO O BINARIO NA TELA
    ------------------------------------
    */

    //Desalocar tipos utilizados
    desalocar_ArqDados(arq_dados);
    desalocar_ArqIndex(arq_index);

    //Fechar arquivos
    fechar_arq_index(arq_index);
    fechar_arq_dados(arq_dados);
    printf("fechamento dos arquivos concluído\n");
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
    



    //checo se existe um arq de indice para aquele campo
    //se sim, faz busca binaria do arquivo de indice
    //se nao, faz busca sequencial no arquivo de dados
}

//Funcionalidade [5]
void delete_from(){

}

//Funcionalidade [6]
void insert_into(){

}

//Funcionalidade [7]
void update(){

}