#include <stdio.h>
#include <stdlib.h>
#include "auxiliares.h"
#include "manipulacao.h"

void create_index(void);

int main(){
    int funcionalidade;
    scanf("%d", &funcionalidade);

    switch(funcionalidade){
        case 3:
            create_index();
            break;
    }

    return 0;
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
    printf("leitura dos dados concluída\n");

    //Com os inputs armazenados, faço 
    //a abertura dos arquivos.
    abrir_arq_dados(arq_dados, "rb");
    abrir_arq_index(arq_index, "wb");
    printf("abertura dos arquivos concluída\n");

    //Ler o cabeçalho do arquivo de dados
    ler_cabecalho_dados(arq_dados);

    //Verificar consistência dos dados do arquivo
    confere_arq_dados(arq_dados);

    alocar_vet_index(arq_dados, arq_index);
    //===========
        //alocar um vetor do tamanho do número de registros a serem indexados - OK
        //ler cada registro e indexá-lo em sua devida posição
        //ordenar vetor de index
        //salvar vetor no arquivo de index
    //===========

    //Desalocar tipos utilizados
    desalocar_ArqDados(arq_dados);
    desalocar_ArqIndex(arq_index);

    //Fechar arquivos
    fechar_arq_index(arq_index);
    fechar_arq_dados(arq_dados);
    printf("fechamento dos arquivos concluído\n");
}