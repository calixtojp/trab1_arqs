/*
	Biblioteca que fornece ferramentas para manipular
	os arquivos de Dados.
*/

#ifndef _ARQDADOS
#define _ARQDADOS
	
	typedef struct Cabecalho cabecalho_t;
	typedef struct Dados dados_t;	

	cabecalho_t *alocar_cabecalho_dados();
	void mostrar_cabecalho_dados(cabecalho_t *cabecalho);
	dados_t *alocar_dados();
	void inicializar_cabecalho(cabecalho_t *cabecalho);
	void arquivo_status(cabecalho_t *cabecalho,char status);
	int get_nroRegArq(cabecalho_t *cabecalho);
	int get_nroRegRem(cabecalho_t *cabecalho);
	int getIdCrime(dados_t *dado);
	void getDataCrime(dados_t *dado, char *dataDestino);
	int getNumeroArtigo(dados_t *dado);
	void getMarcaCelular(dados_t *dado, char *marcaDestino);
	int getCampoInt(dados_t *dado, char *campo);
	char *getCampoStr(dados_t *dado, char *campo);
	int bytesAteCampoIndexado(dados_t *reg, char *campo);
	void cabecalho_nroRegArq_incrementar(cabecalho_t *cabecalho, int qtd);
	void escrever_bin_registro_cabecalho(cabecalho_t *cabecalho, FILE *arq);
	void ler_csv_registro(FILE *arq, dados_t *dados, char *c, int *registro_nulo);
	int ler_csv_campo_int(FILE *arq, char *c, int *registro_nulo);
	void ler_csv_campo_str_fixo(FILE *arq, char *c, char *str_fixo, int tam);
	char *ler_csv_campo_str_variavel(FILE *arq, char *c);
	void escrever_bin_registro_dados(dados_t *dados, FILE *arq, cabecalho_t *cabecalho);
	void escrever_bin_campo_variavel(char *texto, FILE *arq, cabecalho_t *cabecalho);
	cabecalho_t *ler_cabecalho(FILE *arq_bin);
	void sair_fechando(FILE *arq_bin);
	void desalocar_registro(dados_t *registro);
	char status_disponivel(cabecalho_t *cabecalho);
	int existem_registros(cabecalho_t *cabecalho);
	void copia_registro(dados_t *destino, dados_t *origem);
	void mostrar_registros(FILE *arq_bin);
	int get_registro_removido(dados_t *registro);
	int ler_bin_status(FILE *arq_bin, dados_t *registro);
	int ler_bin_registro(dados_t *registro, FILE *arq_bin);
	void ler_bin_campos_fixos(FILE *arq_bin, dados_t *registro, int *flag_chegou_fim);
	void ler_bin_campos_variaveis(FILE *arq_bin, dados_t *registro, int *flag_chegou_fim);
	char *ler_bin_char_variavel(FILE *arq_bin);
	void mostrar_campos(dados_t *registro);
	void mostrar_campo_fixo(char cursor[], int tam_palavra);
	void mostrar_campo_variavel(char *palavra);
	cabecalho_t* ler_dados_cabecalho(FILE *arq_bin);

#endif