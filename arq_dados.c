#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "arq_dados.h"
#include "auxiliares.h"

#define TAM_MAX_BUFFER_INT 10

struct Cabecalho{
	char status;
	long int proxByteOffset;
	int nroRegArq;
	int nroRegRem;
};

struct Dados{
	char removido;
	int idCrime;
	char dataCrime[10];
	int numeroArtigo;
	char marcaCelular[12];
	char *lugarCrime;
	char *descricaoCrime;
	char hashtag;
};

cabecalho_t *alocar_cabecalho_dados(){
	//função que aloca dinamicamente um registro de cabecalho

	cabecalho_t *cabecalho = (cabecalho_t *) malloc(sizeof(cabecalho_t)*1);
    erro(cabecalho);

    return cabecalho;
}

void mostrar_cabecalho_dados(cabecalho_t *cabecalho){
	printf("status:%c\n", cabecalho->status);
	printf("proxByteOffset:%ld\n", cabecalho->proxByteOffset);
	printf("nroRegArq:%d\n", cabecalho->nroRegArq);
	printf("nroRegRem:%d\n", cabecalho->nroRegRem);
}

dados_t *alocar_dados(){
	//função que aloca dinamicamente um registro de dados

	dados_t *dados = (dados_t *) malloc(sizeof(dados_t)*1);
    erro(dados);

    return dados;
}

void inicializar_cabecalho(cabecalho_t *cabecalho){
	//inicializa o cabeçalho para um arquivo binário que ainda não tem conteúdo
	cabecalho->status = '0';
	cabecalho->proxByteOffset = 17;
	cabecalho->nroRegArq = 0;
	cabecalho->nroRegRem = 0;
}

void arquivo_status(cabecalho_t *cabecalho,char status){
	//atualiza o status do arquivo de acordo com o que for informado
	cabecalho->status = status;
}

int get_nroRegArq(cabecalho_t *cabecalho){
	return cabecalho->nroRegArq;
}

int get_nroRegRem(cabecalho_t *cabecalho){
	return cabecalho->nroRegRem;
}

int getIdCrime(dados_t *dado){
	return dado->idCrime;
}

void getDataCrime(dados_t *dado, char *dataDestino){
	strcpy(dataDestino, dado->dataCrime);
}

int getNumeroArtigo(dados_t *dado){
	return dado->numeroArtigo;
}

void getMarcaCelular(dados_t *dado, char *marcaDestino){
	strcpy(marcaDestino, dado->marcaCelular);
}

void cabecalho_nroRegArq_incrementar(cabecalho_t *cabecalho, int qtd){
	//incrementa o número de registros escritos no arquivo binário na quantidade informada
	cabecalho->nroRegArq = cabecalho->nroRegArq + qtd;
}

void escrever_bin_registro_cabecalho(cabecalho_t *cabecalho, FILE *arq){
	//escreve o cabeçalho no arquivo binário
	fwrite(&cabecalho->status,sizeof(char),1,arq);
	fwrite(&cabecalho->proxByteOffset,sizeof(long int),1,arq);
	fwrite(&cabecalho->nroRegArq,sizeof(int),1,arq);
	fwrite(&cabecalho->nroRegRem,sizeof(int),1,arq);
}

cabecalho_t *ler_cabecalho(FILE *arq_bin){
	if(arq_bin == NULL){
		return NULL;
	}

	cabecalho_t *cabecalho_return = alocar_cabecalho_dados();

	if(fread(&(cabecalho_return->status), sizeof(char), 1, arq_bin)!=1){
		printf("Falha no processamento do arquivo.\n");
		return NULL;
	}

	if(fread(&(cabecalho_return->proxByteOffset), sizeof(long int), 1, arq_bin)!=1){
		printf("Falha no processamento do arquivo.\n");
		return NULL;
	}

	if(fread(&(cabecalho_return->nroRegArq), sizeof(int), 1, arq_bin)!=1){
		printf("Falha no processamento do arquivo.\n");
		return NULL;
	}

	if(fread(&(cabecalho_return->nroRegRem), sizeof(int), 1, arq_bin)!=1){
		printf("Falha no processamento do arquivo.\n");
	}

	return cabecalho_return;
}

void ler_csv_registro(FILE *arq, dados_t *dados, char *c, int *registro_nulo){
	//função que lê uma linha completa do arquivo csv, campo a campo

	dados->idCrime = ler_csv_campo_int(arq,c,registro_nulo);

	//se o registro não é nulo, ou seja, uma linha vazia, deve-se continuar lendo-a
	if(*registro_nulo == 0){
		ler_csv_campo_str_fixo(arq,c,dados->dataCrime,10);
		dados->numeroArtigo = ler_csv_campo_int(arq,c,registro_nulo);
		dados->lugarCrime = ler_csv_campo_str_variavel(arq,c);
		dados->descricaoCrime = ler_csv_campo_str_variavel(arq,c);
		ler_csv_campo_str_fixo(arq,c,dados->marcaCelular,12);
	}
}

int ler_csv_campo_int(FILE *arq, char *c, int *registro_nulo){
	//função que os campos com valores inteiros do arquivo csv

	char texto[TAM_MAX_BUFFER_INT];
	int retorno;

	/*no laço é lido, em uma variável auxiliar, caractere a caractere do arquivo csv até encontrar 
	um caractere que indique que a leitura deve ser interrompida*/
	int i = 0;
	*c = getc(arq);
	while(*c != ',' && *c != '\n' && *c != EOF){
		texto[i] = *c;
		i++;
		*c = getc(arq);
	}
	texto[i] = '\0';

	/*Se houver uma quebra no começo da linha, esse if vai reconhecer e impedir
	que o registro continue sendo lido por meio da flag "registro_nulo"*/
	/*Pela ordem do arquivo csv, os inteiros só ficam na primeira e terceira posição
	então um \n ou EOF aqui significa que a linha é vazia ou incompleta*/
	if(*c == '\n' || *c == EOF){
		*registro_nulo = 1; 
	}

	retorno = atoi(texto);

	/*a funcao atoi() retorna 0 caso o campo seja nulo, mas a especificacao 
	do trabalho pede para que inteiros nulos tenham valor -1*/
	if(retorno == 0) retorno = -1;
	
	return retorno;
}

void ler_csv_campo_str_fixo(FILE *arq, char *c, char *str_fixo, int tam){
	//função que lê as strings de tamanho fixo do arquivo csv

	/*no laço é lido, diretamente na string do registro, caractere a caractere do arquivo csv 
	até encontrar um caractere que indique que a leitura deve ser interrompida*/
	int i = 0;
	*c = getc(arq);
	while(*c != ',' && *c !='\n' && *c != EOF){
		str_fixo[i] = *c;		
		i++;
		*c = getc(arq);
	}

	/*como não se pode ter nenhum campo com lixo, 
	aqui completa-se a string com o caractere '$', caso necessário*/
	while(i<tam){
		str_fixo[i] = '$';
		i++;
	}
}

char *ler_csv_campo_str_variavel(FILE *arq, char *c){
	//função que lê os campos com strings de tamanho variável do arquivo csv

	char *texto;
	
	texto = (char *) malloc (1*sizeof(char));

	/*no laço, lê-se, em uma string auxiliar que será passada diretamente para o registro por meio de ponteiros, 
	caractere a caractere do arquivo csv até encontrar um caractere que indique que a leitura deve ser interrompida*/

	//para resolver o problema da variabilidade do tamanho do campo, usa-se a função realloc em toda iteração do laço
	int i = 0;
	*c = getc(arq);
	while(*c != ',' && *c !='\n' && *c != EOF){
		texto = realloc(texto, i+1);
		texto[i] = *c;
		i++;
		*c = getc(arq);
	}	

	//o laço lê o texto diretamente do csv, em que o limitador de campo é uma vírgula
	//como, no arquivo binário, o limitador deve ser o '|', faz-se isso manualmente
	texto = realloc(texto, i+1);
	texto[i] = '|';

	return texto;
}

void escrever_bin_registro_dados(dados_t *dados, FILE *arq, cabecalho_t *cabecalho){
	//função que escreve, no arquivo binário, um registro de dados inteiro

	//inicializando campos que não foram lidos do csv
	dados->removido = '0';
	dados->hashtag = '#';

	fwrite(&dados->removido,sizeof(char),1,arq);
	fwrite(&dados->idCrime,sizeof(int),1,arq);
	fwrite(dados->dataCrime,sizeof(char),10,arq);
	fwrite(&dados->numeroArtigo,sizeof(int),1,arq);
	fwrite(dados->marcaCelular,sizeof(char),12,arq);
	escrever_bin_campo_variavel(dados->lugarCrime,arq,cabecalho);
	escrever_bin_campo_variavel(dados->descricaoCrime,arq,cabecalho);
	fwrite(&dados->hashtag,sizeof(char),1,arq);

	//soma o numero de bytes total dos campos de tamanho fixo
	cabecalho->proxByteOffset += 32;
}

void escrever_bin_campo_variavel(char *texto, FILE *arq, cabecalho_t *cabecalho){
	//função que escreve, no arquivo binário, campos de tamanho variável	

	/*No laço, escreve-se, caractere a caractere, uma string de tamanho variável inteira,
	sendo que seu fim é dado pela presença do caractere '|'*/
	int i=-1;
	do{
		i++;
		fwrite(&texto[i],sizeof(char),1,arq);
	}while(texto[i] != '|');

	//atualizando o proxByteOffset a partir do numero de bytes escritos em cada campo variável
	//A variável 'i' armazena o número de caracteres (cada caractere tem 1 byte de tamanho) escritos -1
	cabecalho->proxByteOffset += (i+1);

	//como as strings de tamanho variável são alocadas dinamicamente a cada leitura, devem ser liberadas a cada escrita.
	free(texto);
}

void sair_fechando(FILE *arq_bin){
	//se um erro for encontrado é preciso finalizar do programa
	//fechando o arquivo binário, uso essa função.
	fclose(arq_bin);
	exit(0);
}

void desalocar_registro(dados_t *registro){
	free(registro->descricaoCrime);
	free(registro->lugarCrime);
}

char status_disponivel(cabecalho_t *cabecalho){
	return cabecalho->status;
}

int existem_registros(cabecalho_t *cabecalho){
	//verifica a existência de registros.
	//retorna 1 se existe pelo menos 1 registro
	//retorna 0 se não existem registros disponíveis
	if(cabecalho->nroRegArq == 0){
		return 0;
	}

	if(cabecalho->nroRegArq == cabecalho->nroRegRem){
		return 0;
	}

	return 1;
}

void copia_registro(dados_t *destino, dados_t *origem){

	printf("vou mostrar a origem\n");
	mostrar_campos(origem);

	destino->removido = origem->removido;
	destino->numeroArtigo = origem->numeroArtigo;
	destino->idCrime = origem->idCrime;
	destino->hashtag = '#';
	strcpy(destino->marcaCelular, origem->marcaCelular);
	strcpy(destino->dataCrime, origem->dataCrime);

	int n;
	//variável que será usada para armazenar o tamanho
	//das strings de tamanho variável
	n = strlen(origem->lugarCrime);
	destino->lugarCrime = alocar_nome(n+1);
	strcpy(destino->lugarCrime, origem->lugarCrime);


	n = strlen(origem->descricaoCrime);
	destino->descricaoCrime = alocar_nome(n+1);
	strcpy(destino->descricaoCrime, origem->descricaoCrime);


	printf("vou mostrar o destino\n");
	mostrar_campos(destino);
}

int ler_bin_registro(dados_t *registro, FILE *arq_bin){
	int chegou_fim = feof(arq_bin);

	if(fread(&(registro->removido), sizeof(char), 1, arq_bin)!=1){
		//Não consegui ler o campo 'removido'
		registro = NULL;
		return 0;
	}

	chegou_fim = 0;
	ler_bin_campos_fixos(arq_bin, registro, &chegou_fim);
	if(chegou_fim != 0){
		//Não conseguiu ler algum dos campos fixos
		printf("não consegui ler algum dos campos fixos\n");
		registro = NULL;
		return 0;
	}

	ler_bin_campos_variaveis(arq_bin, registro, &chegou_fim);
	if(chegou_fim != 0){
		printf("não consegui ler algum dos campos variáveis\n");
		//Não conseguiu ler algum dos campos variáveis
		registro = NULL;
		return 0;
	}

	//ler # no final
	if(fread(&(registro->hashtag), sizeof(char), 1, arq_bin)!=1){
		printf("não consegui ler a hashtag\n");
		registro = NULL;
		return 0;
	}

	//Se leu tudo certo, retorno 1
	return 1;
}

void mostrar_registros(FILE *arq_bin){
	int chegou_fim;//flag que tem 1 se chegou no fim do arquivo ou 0 se não
	chegou_fim = feof(arq_bin);

	while(chegou_fim == 0){

		//alocar espaço de memória para um registro
		dados_t *registro = alocar_dados();

		//ler campo "removido"
		fread(&(registro->removido), sizeof(char), 1, arq_bin);
		
		if(registro->removido == '1'){
			//se o registro foi removido, não preciso ler ele
			pular_registro_removido(arq_bin);
		}else{
			ler_bin_campos_fixos(arq_bin, registro, &chegou_fim);
			if(chegou_fim != 0){
				//Se a flag 'chegou_fim' foi alterada dentro da função
				//ler_bin_campos_fixos, significa que houve um erro na leitura
				//logo, o arquivo chegou em seu fim e devo encerrar o laço

				break;
			}
			
			ler_bin_campos_variaveis(arq_bin, registro, &chegou_fim);
			//a mesma lógia não se aplica aos campos variáveis, pois não
			//é possível ler ter um erro na leitura dos campos variáveis
			//sem antes detectar um erro nos campos fixos (que estão à frente)

			//ler a hashtag no final
			char hashtag;
			fread(&hashtag, sizeof(char), 1, arq_bin);
			
			mostrar_campos(registro);

			//desaloco campos variáveis do registro que foi mostrado
			desalocar_registro(registro);
		}

		//desalocar registro
		free(registro);

		//atualizo a flag
		chegou_fim = feof(arq_bin);
	}
}

int get_registro_removido(dados_t *registro){
	if(registro->removido == '1'){
		return 1;
	}else{
		return 0;
	}
}

int ler_bin_status(FILE *arq_bin, dados_t *registro){
	if(fread(&(registro->removido), sizeof(char), 1, arq_bin)==1){
		return 1;
	}else{
		return 0;
	}
}

void ler_bin_campos_fixos(FILE *arq_bin, dados_t *registro, int *flag_chegou_fim){
	//Lê e armazena no ponteiro "registro" todos os campos
	//de tamanho fixo (exceto o campo 'char removido', que já foi lido)

	//Se ocorre qualquer erro na leitura de um dos campos - isto é, 
	//fread não retorna a quantidade de dados que foi solicitada para leitura -
	//então atualizo a flag_chegou_fim, pois cheguei no fim do arquivo
	if(fread(&(registro->idCrime), sizeof(int), 1, arq_bin)!=1){
		*flag_chegou_fim = 1;
		return;
	}

	if(fread(&(registro->dataCrime), sizeof(char), 10,arq_bin)!=10){
		*flag_chegou_fim = 1;
		return;
	}	

	if(fread(&(registro->numeroArtigo), sizeof(int), 1, arq_bin)!=1){
		*flag_chegou_fim = 1;
		return;
	}

	if(fread(&(registro->marcaCelular), sizeof(char), 12, arq_bin)!=12){
		*flag_chegou_fim = 1;
		return;
	}
}

void ler_bin_campos_variaveis(FILE *arq_bin, dados_t *registro, int *flag_chegou_fim){
	//lê e armazena no *registro os campos de tamanho variável
	registro->lugarCrime = ler_bin_char_variavel(arq_bin);
	registro->descricaoCrime = ler_bin_char_variavel(arq_bin);
}

char *ler_bin_char_variavel(FILE *arq_bin){
	//função que retorna a palavra de tamanho variável a ser lida

	//no caso de existir um campo que foi removido, o vetor_retorno terá
	//somente um espaço o qual será ocupado por um '\0'.
	//Desse modo, se existe um vetor que tem somente um '\0' armazenado, sei
	//que esse é um campo NULO

	int tam_vet_ret = 0;//tamanho do vetor_retorno
	char *vetor_retorno = (char*)malloc(sizeof(char));//vetor que será retornado
	char cursor;
	fread(&cursor, sizeof(char), 1, arq_bin);
	while(cursor != '|'){
		vetor_retorno[tam_vet_ret] = cursor;
		++tam_vet_ret;
		vetor_retorno = realloc(vetor_retorno, sizeof(char)*(tam_vet_ret+1));
		fread(&cursor, sizeof(char), 1, arq_bin);
	}
	vetor_retorno[tam_vet_ret] = '\0';
	return vetor_retorno;
}

void mostrar_campos(dados_t *registro){
	//Método que mostra um registroa

	if(registro->idCrime != -1){
		printf("%d, ", registro->idCrime);
	}else{
		mensagem_NULO();
	}

	mostrar_campo_fixo((registro->dataCrime), 10);
	printf(", ");

	if(registro->numeroArtigo != -1){
		printf("%d, ", registro->numeroArtigo);
	}else{
		mensagem_NULO();
	}

	mostrar_campo_variavel(registro->lugarCrime);
	printf(", ");

	mostrar_campo_variavel(registro->descricaoCrime);
	printf(", ");

	mostrar_campo_fixo((registro->marcaCelular), 12);
	printf("\n");
}

void mostrar_campo_fixo(char cursor[], int tam_palavra){
	int letras_validas = 0;//indica a quantidade de letras válidas de uma palavra

	while(cursor[letras_validas] != '$' && (letras_validas < tam_palavra)){
		printf("%c", cursor[letras_validas]);
		letras_validas++;
	}
	if(letras_validas == 0){//ou seja, não existe palavra
		printf("NULO");
	}
}

void mostrar_campo_variavel(char *palavra){
	if(palavra[0] == '\0'){
		//isso significa que não existe palavra, ou seja
		//é um campo NULO
		printf("NULO");
	}else{
		printf("%s", palavra);
	}
}

cabecalho_t *ler_bin_cabecalho(FILE *arq_bin){
	//Lê e retorna um ponteiro para o cabeçalho do arquivo 
	//binário que fora passado como argumento
	erro(arq_bin);

	cabecalho_t *cabecalho_retorno = alocar_cabecalho_dados();

	//Se algum campo do cabeçalho não for lido, retorno uma 
	//mensagem de erro
	if(fread(&(cabecalho_retorno->status), sizeof(char), 1, arq_bin)!=1){
		printf("Falha no processamento do arquivo.\n");
		return NULL;
	}

	if(fread(&(cabecalho_retorno->proxByteOffset), sizeof(long int), 1, arq_bin)!=1){
		printf("Falha no processamento do arquivo.\n");
		return NULL;
	}

	if(fread(&(cabecalho_retorno->nroRegArq), sizeof(int), 1, arq_bin)!=1){
		printf("Falha no processamento do arquivo.\n");
		return NULL;
	}

	if(fread(&(cabecalho_retorno->nroRegRem), sizeof(int), 1, arq_bin)!=1){
		printf("Falha no processamento do arquivo.\n");
		return NULL;
	}

	return cabecalho_retorno;
}