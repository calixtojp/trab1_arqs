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

#include "arq_dados.h"
#include "arq_indice.h"

typedef struct ArqDados ArqDados_t;
typedef struct ArqIndex ArqIndex_t;
typedef struct InfoBusca InfoBusca_t;

typedef void (*FncAcao) (ArqDados_t*, ArqIndex_t*, dados_t*, long int);
typedef void (*FncFinaliza) (void*, int);
typedef long int (*FncGetByteOffSet) (void*, int);
typedef int (*FncCampoNulo) (void*);
typedef void* (*FncGetCampoIndexIndexado) (dados_t*, char*);
typedef void (*FncSetDadoIndx) (void*, long int, void*);
typedef void (*FncSetVetIndx) (void*, int, void*);
typedef void (*FncOrdemTipo) (void*, int);

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
void printa_busca(ArqDados_t *arq_dados, ArqIndex_t *arq_index, dados_t *reg, long int byteOffSet);
int get_nroRegValidos(ArqDados_t *arq_dados);
char *getNomeArqIndex(ArqIndex_t *arq_index);
char *getNomeArqDados(ArqDados_t *arq_dados);
void alocar_vet_index(ArqIndex_t *arq_index, unsigned int nroRegValidos);
void realocar_vet_index(ArqIndex_t *arq_index, int original, int acrescimo);
int indexaRegistro(ArqDados_t *arq_dados, ArqIndex_t *arq_index, int pos_reg, long int *byteOffSetAnt);
void desindexaReg(int pos, cabecalho_indx_t *cabecalho, void *vetIndx);
void ordenaVetIndex(ArqIndex_t *arq_index, int qntd_reg);
void escreveVetIndex(ArqIndex_t *arq_index, int inicio, int fim);
void terminaEscritaIndex(ArqIndex_t *arq_index, int qntdReg);
void terminaEscritaDados(ArqDados_t *arq_dados, int qntdReg);
int get_nroRegIndex(ArqIndex_t *arq_index);
void mostrar_arq_index(ArqIndex_t *arq_index);
void inserirReg(ArqDados_t *arq_dados, ArqIndex_t *arq_index, dados_t *reg, int pos);
int inserirRegStdin(ArqDados_t *arq_dados, ArqIndex_t *arq_index, int pos);
int existe_index(InfoBusca_t *criterios, ArqIndex_t *arq_index);
void percorrer_index(FncGetByteOffSet get_byteOffset, int pos_prim, int qtd_reg_val, void *vetor, 
                    ArqDados_t *arq_dados, ArqIndex_t *arq_index,InfoBusca_t *criterios, FncAcao acao, FncFinaliza final);
void busca_bin_index(ArqIndex_t *arq_index, ArqDados_t *arq_dados, int pos_chave, InfoBusca_t *criterios, FncAcao acao, FncFinaliza final);
void busca_seq_dados(ArqDados_t *arq_dados, ArqIndex_t *arq_index,InfoBusca_t *criterios, FncAcao acao, FncFinaliza final);
int get_nroRegIndex(ArqIndex_t *arq_index);
void mostrar_arq_index(ArqIndex_t *arq_index);
InfoBusca_t *ler_criterios_busca();
InfoBusca_t *alocar_InfoBusca(int qtd_crit);
void desalocar_InfoBusca(InfoBusca_t *informacao);
void processaRegistros(ArqDados_t *arq_dados, ArqIndex_t *arq_index, InfoBusca_t *criterios, FncAcao acao, FncFinaliza final);
void editarRegStdin(ArqIndex_t *arq_index, ArqDados_t *arq_dados, int *qtdRegDados,int *qtdRegInx);
void achouReg(void *arq_index, int flag);
void nada(void *ponteiro, int flag);
int testarStatusIndex(ArqIndex_t *arq_index);
int testarStatusDados(ArqDados_t *arq_dados);
void deletarRegistro(ArqDados_t *arq_dados, ArqIndex_t *arq_index, dados_t *registro, long int byteOffset);

#endif