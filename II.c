#include <stdio.h>



const char path[50];
printf("Le chemin/\n");
scanf(%s, &path);


int ouvrir_file(const char* path){
    FILE *f = fopen(path, "rb");
    if(f==NULL) return NULL;
    
}

//TODO: fncts pour passer de file à texte code mais genre avec le bon registre sans la ponctuation et tout le reste