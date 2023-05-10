#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "auxiliares.h"
#include "manipulacao.h"

#define TAM_MAX_NOME 50
#define TAM_MAX_VALOR 50

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

    //Carrego o arquivo de indice na memoria primaria
    ler_cabecalho_arq_index(arq_index);
    ler_dados_arq_index(arq_index);

    for(int i=1; i<=n; i++){
        printf("Resposta para a busca %d\n",i);

    	int m;
    	scanf("%d",&m);

    	char **vet_nomes = alocar_vetor_string(m,TAM_MAX_NOME);
    	char **vet_vals_str = alocar_vetor_string(m, TAM_MAX_VALOR);
    	int *vet_vals_int = malloc(sizeof(int)*m);

    	for(int j=0; j<m; j++){
    		scanf("%s",vet_nomes[j]);
    		if(ehInteiro(vet_nomes[j])){
    			//se o campo for um int
				scanf("%d",&vet_vals_int[j]); //leio o valor

    			//indico no vetor de valores string que o valor deve ser procurado no vetor de int
    			strcpy(vet_vals_str[j],"int");
    		}else{
    			//se nao for int, é string
    			ler_aspas_string(vet_vals_str[j]); //leio o valor

    			//indico no vetor de valores int que o valor deve ser procurado no vetor de strings
    			vet_vals_int[j] = -1;
    		}
    	}

        int existe = existe_index(m,vet_nomes,arq_index);
        /*Se existe arquivo de indice para um dos campos a serem buscados,
        a variável 'existe' recebe o indice do nome desse campo no vet_nomes.
        Se não existe, a variável recebe -1.*/

        if(existe >= 0 ){
            /*se existe arquivo de index para um dos campos que se deseja 
            buscar, faz-se busca binária no arquivo de indice*/

            busca_bin_index(arq_index,arq_dados,existe,vet_nomes,vet_vals_str,vet_vals_int,m);

        }else{
            //se não, faz-se busca sequencial no arquivo de dados
            busca_seq_dados(arq_dados,m,vet_vals_str,vet_vals_int);
        }

		//Desalocar tipos utilizados    	
	    desalocar_vetor_string(vet_vals_str,m);
    	desalocar_vetor_string(vet_nomes,m);
    	free(vet_vals_int);
    }

    //Desalocar tipos utilizados
    desalocar_ArqDados(arq_dados);
    desalocar_ArqIndex(arq_index);

    //Fechar arquivos
    fechar_arq_index(arq_index);
    fechar_arq_dados(arq_dados);

    //DENTRO DO LOOP DE N BUSCAS:
    //checo se existe um arq de indice para pelo menos 1 dos campos chave de busca
    /*se sim, 
    -faz busca binaria do arquivo de indice (com a funcao de comparacao)
	-acha o primeiro (todos que satisfazem a condição do arq de indice vao estar ordenados)
	-segue sequencialmente essa lista, testa pros outros criterios (com a função de comp) e printa ou nao
	*/

    /*se nao,
    -faz busca sequencial no arquivo de dados (com a funcao de comparacao) 
    -le tudo até o fim e printa todos q achar no caminho
    */

    /*
    Criar função de comparação, que está preparada pra comparar 
    todos os campos da struct dados, mas os q nao for usar, vai marcar como true
    pra poder passar no and com qualquer coisa (1 and A == A).
    Faço variaveis booleanas de verificacao de cada campo, e inicializo todas com 1.
    se um dos campos for uma das m chaves de busca da entrada, eu troco o 1 pela comparação*/
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