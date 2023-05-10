/*
	Biblioteca que fornece ferramentas para utilizar
	os arquivos de Índice. teste
*/

#ifndef _ARQINDICE
#define _ARQINCIDE

    typedef struct Cabecalho_indx cabecalho_indx_t;
    typedef struct Dados_indx_int dados_indx_int_t;
    typedef struct Dados_indx_str dados_indx_str_t;
    cabecalho_indx_t *alocar_cbl_indx(void);
    dados_indx_int_t *alocDadoIndxInt(void);
    dados_indx_str_t *alocDadoIndxStr(void);
    dados_indx_int_t **aloc_vet_indx_DadoInt(int nroRegValidos);
    dados_indx_str_t **aloc_vet_indx_DadoStr(int nroRegValidos);   
    char *alocarCampoIndexado(void);
    void setCabecalhoIndex(cabecalho_indx_t *cabecalho, const char status, int qtdReg);
    cabecalho_indx_t *get_cabecalho_indx(FILE *arqIndex);
    void setDadoIndxInt(void *dado, long int byteOffSet, void *valor);
    void setDadoIndxStr(void *dado, long int byteOffSet, void *valor);
    void setVetIndx_int(void *vet, int pos, void *dado);
    void setVetIndx_str(void *vet, int pos, void *dado);
    void copiaDadoIndex_int(void *destino, void *origem);
    void copiaDadoIndex_str(void *destino, void *origem);
    int getTamCabecalhoIndx(void);
    int getTamDadoIndx_int(void);
    int getTamDadoIndx_str(void);
    int get_qtdReg(cabecalho_indx_t *cabecalho);
    cabecalho_indx_t *ler_index_cabecalho(FILE *arq);
    dados_indx_int_t **ler_index_dado_int(FILE *arq, cabecalho_indx_t *cabecalho);
    dados_indx_str_t **ler_index_dado_str(FILE *arq, cabecalho_indx_t *cabecalho);
    void mostraRegIndx_int(dados_indx_int_t *dado);
    void mostraRegIndx_str(dados_indx_str_t *dado);
    void mostraVetInt(void *vet_generico, int qntd_reg);
    void mostraVetStr(void *vet_generico, int qntd_reg);
    void ordenaVetIndex_int(void *vetor_generico, int qntd_reg);
    void ordenaVetIndex_str(void *vetor_generico, int qntd_reg);
    void escreveCabecalhoIndex(FILE *arqIndex, cabecalho_indx_t *cabecalho);
    int busca_bin_int(dados_indx_int_t **vetor, cabecalho_indx_t *cabecalho,int chave, int *qtd_reg_val);
    void escreveVetIndx_int(FILE *arqIndex, void *vet_indx_int, int pos);
    void escreveVetIndx_str(FILE *arqIndex, void *vet_indx_str, int pos);
    int busca_bin_str(dados_indx_str_t **vetor, cabecalho_indx_t *cabecalho, char *chave, int *qtd_reg_val);
    long int get_byteOffset_int(void *ponteiro, int pos);
    long int get_byteOffset_str(void *ponteiro, int pos);
    int testar_status_indx(cabecalho_indx_t *cabecalho);
 
#endif