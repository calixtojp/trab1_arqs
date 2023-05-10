#ifndef _AUXILIARES
#define _AUXILIARES

	void binarioNaTela(char *nomeArquivoBinario);
	void readline(char *string);
	void ler_aspas_string(char *str);
	void erro(void *ponteiro);
	char **alocar_vetor_string(int n_elem, int n_chars);
	void desalocar_vetor_string(char **vetor, int n_elem);
	int ehInteiro(char *campo);
	char *truncar(char *original);
	int comparar_strings(char *s1, char *s2);
	int comparar_n_chars(char *s1, char *s2, int n);

	/* --- T0, ARRUMAR -------------------*/
	int tamanhoStr(char *string);
	char *alocar_nome(int tamanho_nome);
	void pular_campos_fixos(FILE *arq_bin);
	void pular_ate_fim_registro(FILE *arq_bin);
	void pular_registro_removido(FILE *arq_bin);
	void pular_cabecalho_csv(FILE *arq);
	void mensagem_NULO(void);
	
#endif