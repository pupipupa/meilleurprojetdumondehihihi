//*les included:

#include "projet.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

//* le main:

int main(){
    return 0;
}



//NOTE: ANCIEN MAIN QU IL FAUT ADAPTER:

/*

int main(void){
        
    char *content = ouvrir_file("test.txt");
    if(content == NULL){
        fprintf(stderr, "Erreur: ouvrir_file a échoué\n");
        return 1;
    }

    normalisation_texte_v2(content);
    printtestcontenu(content);

    free(content);
    return 0;
}


*/