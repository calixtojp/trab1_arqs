/*
	Biblioteca que fornece ferramentas para manipular
	os arquivos de Índice.
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
    void setCabecalhoIndex(cabecalho_indx_t *cabecalho, const char status);
    void setDadoIndxInt(dados_indx_int_t *dado, long int byteOffSet, int valor);
    void setDadoIndxStr(dados_indx_str_t *dado, long int byteOffSet, char *valor);
    void copiaDadoIndex_int(dados_indx_int_t *destino, dados_indx_int_t *origem);
    void copiaDadoIndex_str(dados_indx_str_t *destino, dados_indx_str_t *origem);
    void mostraRegIndx_int(dados_indx_int_t *dado);
    void mostraRegIndx_str(dados_indx_str_t *dado);
    void mostraVetInt(void *vet_generico, int qntd_reg);
    void mostraVetStr(void *vet_generico, int qntd_reg);
    void ordenaVetIndex_int(void *vetor_generico, int qntd_reg);
    void ordenaVetIndex_str(void *vetor_generico, int qntd_reg);
    void escreveCabecalhoIndex(FILE *arqIndex, cabecalho_indx_t *cabecalho);

#endif