#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "arq_dados.h"
#include "auxiliares.h"

#define TAM_MAX_CABECALHO_CSV 100

void binarioNaTela(char *nomeArquivoBinario){
    /*
     * Você não precisa entender o código dessa função.
     *
     * Use essa função para comparação no run.codes.
     * Lembre-se de ter fechado (fclose) o arquivo anteriormente.
     * Ela vai abrir de novo para leitura e depois fechar
     * (você não vai perder pontos por isso se usar ela).
     */

    unsigned long i, cs;
    unsigned char *mb;
    size_t fl;
    FILE *fs;
    if (nomeArquivoBinario == NULL || !(fs = fopen(nomeArquivoBinario, "rb"))) {
        fprintf(stderr,
                "ERRO AO ESCREVER O BINARIO NA TELA (função binarioNaTela): "
                "não foi possível abrir o arquivo que me passou para leitura. "
                "Ele existe e você tá passando o nome certo? Você lembrou de "
                "fechar ele com fclose depois de usar?\n");
        return;
    }
    fseek(fs, 0, SEEK_END);
    fl = ftell(fs);
    fseek(fs, 0, SEEK_SET);
    mb = (unsigned char *)malloc(fl);
    fread(mb, 1, fl, fs);

    cs = 0;
    for (i = 0; i < fl; i++) {
        cs += (unsigned long)mb[i];
    }
    printf("%lf\n", (cs / (double)100));
    free(mb);
    fclose(fs);
}

void readline(char *string) {
    char c = 0;

    do {
        c = (char)getchar();
    } while (c == '\n' || c == '\r');

    int i = 0;

    do {
        string[i] = c;
        i++;
        c = getchar();
    } while (c != '\n' && c != '\r');

    string[i] = '\0';
}

void ler_aspas_string(char *str) {
    /*
     * Use essa função para ler um campo string delimitado entre aspas (").
     * Chame ela na hora que for ler tal campo. Por exemplo:
     *
     * A entrada está da seguinte forma:
     *    nomeDoCampo "MARIA DA SILVA"
     *
     * Para ler isso para as strings já alocadas str1 e str2 do seu programa,
     * você faz:
     *    scanf("%s", str1); // Vai salvar nomeDoCampo em str1
     *    scan_quote_string(str2); // Vai salvar MARIA DA SILVA em str2
     *                             // (sem as aspas)
     */

    char R;

    while ((R = getchar()) != EOF && isspace(R)) {
        ;  // ignorar espaços, \r, \n...
    }

    if (R == 'N' || R == 'n') {  // campo NULO
        getchar();
        getchar();
        getchar();  // ignorar o "ULO" de NULO.
        strcpy(str, "");  // copia string vazia
    } else if (R == '\"') {
        if (scanf("%[^\"]", str) != 1) {  // ler até o fechamento das aspas
            strcpy(str, "");
        }
        getchar();  // ignorar aspas fechando
    } else if (R != EOF) {  // vc tá tentando ler uma string
                            // que não tá entre aspas!
                            // Fazer leitura normal %s então,
                            // pois deve ser algum inteiro ou algo assim...
        str[0] = R;
        scanf("%s", &str[1]);
    } else {  // EOF
        strcpy(str, "");
    }
}

void erro(void *ponteiro){
    //verifica a existência de algum erro ao longo das funcionalidades
    if(ponteiro == NULL){
        exit(0);
    }
}

char **alocar_vetor_string(int n_elem, int n_chars){
    char **vetor = malloc(sizeof(char *)*n_elem);
    for(int i=0; i<n_elem; i++){
        vetor[i] = malloc(sizeof(char)*n_chars);
    }

    return vetor;
}

void desalocar_vetor_string(char **vetor, int n_elem){
    for(int i=0; i<n_elem; i++){
        free(vetor[i]);
    }
    free(vetor);
}

int ehInteiro(char *campo){
    //funcao que identifica se um campo eh do tipo inteiro ou nao
    if((strcmp(campo,"idCrime") == 0) || (strcmp(campo,"numeroArtigo") == 0)){
        return 1;//eh inteiro
    }else{
        return 0;//nao eh inteiro
    }
}

char *truncar(char *original){
    //funcao que faz uma string ter exatos 12 caracteres
    char *truncado = malloc(sizeof(char)*12);

    int tam = strlen(original);

    if(tam>=12){
        //se a string tem mais que 12 chars, entao armazeno só até o char 12
        //se ela tem exatamente 12 chars, eu copio-os apenas
        for(int i=0; i<12; i++){
            truncado[i] = original[i];
        }
    }else if(tam<12){
        //se a string nao ocupa 12 chars, preenche-se com '$' até ter 12 chars
        for(int i=0; i<tam; i++){
            truncado[i] = original[i];
        }
        for(int i=tam; i<12; i++){
            truncado[i] = '$';
        }
    }

    return truncado;
} 

/* --- T0, ARRUMAR -------------------*/
int tamanhoStr(char *string){
    int tamanho = 0;
    char cursor;
    do{
        cursor = string[tamanho];
        printf("cursor:%c|tam:%d\n", cursor, tamanho);
        tamanho++;
    }while(cursor != '|');
    return tamanho;
}

char *alocar_nome(int tamanho_nome){
    char *nome = malloc(sizeof(char) * tamanho_nome);
    return nome;
}

void pular_campos_fixos(FILE *arq_bin){
    //pula os campos fixos exceto o campo 'removido'
    int qntd_bytes_pular = 2*sizeof(int) + 23*sizeof(char);
    fseek(arq_bin, qntd_bytes_pular, SEEK_CUR);
}

void pular_ate_fim_registro(FILE *arq_bin){
    //função que pula até o fim de um registro
    char cursor;
    do{
        fread(&cursor, sizeof(char), 1, arq_bin);
    }while(cursor != '#');
}

void pular_registro_removido(FILE *arq_bin){
    pular_campos_fixos(arq_bin);
    pular_ate_fim_registro(arq_bin);
}

void pular_cabecalho_csv(FILE *arq){
    //função que lê e ignora a primeira linha do arquivo csv

    char texto[TAM_MAX_CABECALHO_CSV];

    //lê-se, caractere a caractere, a linha até encontrar a quebra de linha
    int i=-1;
    do{
        i++;
        fscanf(arq,"%c",&texto[i]);
    }while(texto[i] != '\n');
}

void mensagem_NULO(void){
    printf("NULO, ");
}