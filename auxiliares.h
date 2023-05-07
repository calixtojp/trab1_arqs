#ifndef _AUXILIARES
#define _AUXILIARES

	void binarioNaTela(char *nomeArquivoBinario);
	void readline(char *string);
	void scan_quote_string(char *str);
	void erro(void *ponteiro);

	/* --- T0, ARRUMAR -------------------*/
	int tamanhoStr(char *string);
	char *alocar_nome(int tamanho_nome);
	void pular_campos_fixos(FILE *arq_bin);
	void pular_ate_fim_registro(FILE *arq_bin);
	void pular_registro_removido(FILE *arq_bin);
	void pular_cabecalho_csv(FILE *arq);
	void mensagem_NULO(void);
	
#endif