#include <stdio.h>
#include <string.h>

int main(){
	char nomes[6][50] = {
		"aaa",
		"bbb",
		"ccc",
		"ddd",
		"eee",
		"fff"
	};

    for(int i = 0; i < 6; ++i){
        char ent[50];
        scanf("%s", ent);
        printf("nome %d:%s\n", i,nomes[i]);
        if(strcmp(nomes[i], ent)==0){
            printf("nome igual");
        }
    }

    return 0;
}