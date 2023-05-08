/*
        Biblioteca que utiliza as ferramentas fornecidas
    nas bibliotecas de manipulação dos arquivos de
    Índice e de Dados para manipula-los.
        Essa manipulação ocorre através dos tipos ArqDados
    (referente ao arquivo de dados) e o tipo ArqIndex (referente)
    ao arquivo de Index
*/

#ifndef _MANIPULACAO
#define _MANIPULACAO

typedef struct ArqDados ArqDados_t;
typedef struct ArqIndex ArqIndex_t;

ArqDados_t *alocar_arq_dados(void);
ArqIndex_t *alocar_arq_index(void);
void ler_nome_arq_dados(ArqDados_t *arq_dados);
void ler_nome_arq_index(ArqIndex_t *arq_index);
void ler_campoIndexado(ArqIndex_t *arq_index);
void ler_tipoDado(ArqIndex_t *arq_index);
void abrir_arq_dados(ArqDados_t *arq_dados, const char *tipo_leitura);
void abrir_arq_index(ArqIndex_t *arq_index, const char *tipo_leitura);
void desalocar_ArqDados(ArqDados_t *arq_dados);
void desalocar_ArqIndex(ArqIndex_t *arq_index);
void fechar_arq_dados(ArqDados_t *arq_dados);
void fechar_arq_index(ArqIndex_t *arq_index);
void ler_cabecalho_dados(ArqDados_t *arq_dados);
void mostrar_cabecalhoDados(ArqDados_t *arq_dados);
void confere_arq_dados(ArqDados_t *arq_dados);
int get_nroRegValidos(ArqDados_t *arq_dados);
void set_qtdReg(ArqIndex_t *arq_index, int qtdReg);
void alocar_vet_index(ArqIndex_t *arq_index, unsigned int nroRegValidos);
int indexaRegistro(ArqDados_t *arq_dados, ArqIndex_t *arq_index, int pos_reg);
void ordenaVetIndex(ArqIndex_t *arq_index, int qntd_reg);
void escreveVetIndex(ArqIndex_t *arq_index, int inicio, int fim);
int existe_index(int m, char **vet_nomes, ArqIndex_t *arq_index);
void busca_bin_index(ArqIndex_t *arq_index, int pos, char **vet_vals_str, int *vet_vals_int);
void busca_seq_dados(ArqDados_t *arq_dados, int pos, char **vet_vals_str, int *vet_vals_int);

#endif