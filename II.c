#include <stdio.h>



const char path[50];
printf("Le chemin/\n");
scanf(%s, &path);


int ouvrir_file(const char* path){
    FILE *f = fopen(path, "rb");
    if(f==NULL) return 0;
    //TODO: créér un tableau des pointeurs sur le char 
    //TODO: alors du coup il reste d'alluer de la mémoire 
    //TODO: mettre dans cette mémoire les bytes

    //NOTE; free?
    

    fclose(&*(f));
}

//TODO: fncts pour passer de file à texte code mais genre avec le bon registre sans la ponctuation et tout le reste