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
    typedef struct ConexaoReg ConexaoReg_t;

    ArqDados_t *alocar_arq_dados(void);
    ArqIndex_t *alocar_arq_index(void);
    ConexaoReg_t *alocar_cnx(ArqIndex_t *arq_index);
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
    char *getCampoIndexado(ArqIndex_t *arq_index);
    int get_nroRegValidos(ArqDados_t *arq_dados);
    void alocar_vet_index(ArqIndex_t *arq_index, unsigned int nroRegValidos);
    int getDado(ConexaoReg_t *cnx_reg, ArqDados_t *arq_dados, int pular);
    void escreverVetIndex(ConexaoReg_t *cnx_reg, ArqIndex_t *arq_index, int pos);

#endif