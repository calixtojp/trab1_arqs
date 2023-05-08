//Lucas de Souza Brandão, 13695021
//João Pedro Soares de Azevedo Calixto 13732011

#include <stdio.h>
#include <stdlib.h>
#include "funcionalidades.h"

/*coisas que tem q mudar:
-ver quais funções vai ficar no auxiliares e quais ficam em arq_dados ou arq_indice
-trocar cabecalho do arq_index
-ler_bin_campos_variaveis -> fazer retornar int ao inves de passar por parametro
-Colocar apenas as mensagens de erro que a Cristina manda
*/

int main(){
    int funcionalidade;
    scanf("%d", &funcionalidade);

    switch(funcionalidade){
        case 3:
            create_index();
            break;
        case 4:
            where();
            break;
        case 5:
            delete_from();
            break;
        case 6:
            insert_into();
            break;
        case 7:
            update();
            break;
    }

    return 0;
}
