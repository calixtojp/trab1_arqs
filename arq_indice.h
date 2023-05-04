/*
	Biblioteca que fornece ferramentas para utilizar
	os arquivos de Índice.
*/

#ifndef _ARQINDICE
#define _ARQINCIDE

    typedef struct Cabecalho_indx cabecalho_indx_t;
    typedef struct Dados_indx_int dados_indx_int_t;
    typedef struct Dados_indx_str dados_indx_str_t;
    cabecalho_indx_t *alocar_cbl_indx(void);
    dados_indx_int_t **aloc_vet_DadoInt(int nroRegValidos);
    dados_indx_str_t **aloc_vet_DadoStr(int nroRegValidos);   
    void set_CnxInt(dados_indx_int_t *dado, long int byteOffSet, int valor);
    void set_CnxStr(dados_indx_str_t *dado, long int byteOffSet, char *valor);
    void copiaDadoIndex_int(dados_indx_int_t *destino, dados_indx_int_t *origem);
    void copiaDadoIndex_str(dados_indx_str_t *destino, dados_indx_str_t *origem);

#endif