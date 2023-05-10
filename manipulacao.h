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
typedef struct InfoBusca InfoBusca_t;

ArqDados_t *alocar_arq_dados(void);
ArqIndex_t *alocar_arq_index(void);
void ler_nome_arq_dados(ArqDados_t *arq_dados);
void ler_nome_arq_index(ArqIndex_t *arq_index);
void ler_campoIndexado(ArqIndex_t *arq_index);
void ler_tipoDado(ArqIndex_t *arq_index);
void abrir_arq_dados(ArqDados_t *arq_dados, const char *tipo_leitura);
void abrir_arq_index(ArqIndex_t *arq_index, const char *tipo_leitura);
void ler_cabecalho_arq_index(ArqIndex_t *arq_index);
void ler_dados_arq_index(ArqIndex_t *arq_index);
void desalocar_ArqDados(ArqDados_t *arq_dados);
void desalocar_ArqIndex(ArqIndex_t *arq_index);
void fechar_arq_dados(ArqDados_t *arq_dados);
void fechar_arq_index(ArqIndex_t *arq_index);
void ler_cabecalho_dados(ArqDados_t *arq_dados);
int ler_cabecalho_index(ArqIndex_t *arq_index);
int getTamCabecalhoDados(ArqDados_t *arq_dados);
void mostrar_cabecalhoDados(ArqDados_t *arq_dados);
void confere_arq_dados(ArqDados_t *arq_dados);
int get_nroRegValidos(ArqDados_t *arq_dados);
char *getNomeArqIndex(ArqIndex_t *arq_index);
void alocar_vet_index(ArqIndex_t *arq_index, unsigned int nroRegValidos);
int indexaRegistro(ArqDados_t *arq_dados, ArqIndex_t *arq_index, int pos_reg, long int *byteOffSetAnt);
void ordenaVetIndex(ArqIndex_t *arq_index, int qntd_reg);
void escreveVetIndex(ArqIndex_t *arq_index, int inicio, int fim);
void terminaEscritaIndex(ArqIndex_t *arq_index, int qntdReg);
int existe_index(InfoBusca_t *criterios, ArqIndex_t *arq_index);
long int *busca_bin_index(ArqIndex_t *arq_index, ArqDados_t *arq_dados, int pos_chave, InfoBusca_t *criterios, int *cont_reg_vet);
long int *busca_seq_dados(ArqDados_t *arq_dados, InfoBusca_t *criterios, int *cont_reg_vet);
long int *percorrer_index(long int (*get_byteOffset)(void *,int), int pos_prim, int qtd_reg_val, void *vetor, 
                    ArqDados_t *arq_dados, InfoBusca_t *criterios, int *cont_reg_vet);
int get_nroRegIndex(ArqIndex_t *arq_index);
void mostrar_arq_index(ArqIndex_t *arq_index);
InfoBusca_t *ler_criterios_busca();
InfoBusca_t *alocar_InfoBusca(int qtd_crit);
void desalocar_InfoBusca(InfoBusca_t *informacao);
void busca(ArqDados_t *arq_dados, ArqIndex_t *arq_index, int qtd_buscas);
long int *qual_busca(ArqDados_t *arq_dados, ArqIndex_t *arq_index, InfoBusca_t *criterios, int *cont_reg_vet);
int testar_status(ArqIndex_t *arq_index, ArqDados_t *arq_dados);


#endif