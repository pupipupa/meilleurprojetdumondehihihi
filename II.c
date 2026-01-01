#include <stdio.h>
#include <stdlib.h>


char* ouvrir_file(const char* path){
    FILE *f;
    long len_long; //NOTE: FTELL выдает всегда ЛОНГ 
    //unsigned long long fread_resultat; //смотри не ансигнед лонг ебанный а как раз таки сайз ти ебанный тоже 
    size_t fread_resultat;
    
    f = fopen(path, "rb");

    if(f==NULL){
        perror("fopen");
        return NULL;}// не 0 а нулль 

    int fseek_resultat = fseek(f, 0, SEEK_END);

    if(fseek_resultat != 0){
        fprintf(stderr, "Erreur: le fseek n'est pas passé \n");
        fclose(f); 
        return NULL;
    }
    //proverka

    len_long = ftell(f);

    if(len_long < 0){
        fprintf(stderr, "Erreur: le ftell n'est pas passé \n");
        fclose(f); 
        return NULL;
    }

    size_t len = (size_t)len_long; //size_t не бывает нешативным
    // ПРОВЕРКА ЕЛАСИ СРАБОТАЛО 

    rewind(f);

    char* ch = (char*)malloc(len+1); //ANCHOR: НИКОГДА НАХУЙ НЕ ЗАБЫВАЙ +1 ПЖЖЖЖЖЖЖЖЖЖ ДЛЯ '\0'
    // ПРОВЕРКА НА МАЛЛОК 
    if(ch == NULL){
        fprintf(stderr, "Erreur: la mémoire pas alloué\n");
        fclose(f);
        return NULL;
    }

    fread_resultat = fread(ch, 1, len, f);
    //Ты ожидаешь len. Если меньше — ошибка → free(ch) → fclose(f) → NULL
    if(fread_resultat != len){
        //printferror
        fprintf(stderr, "Erreur: le fread n'est pas passé \n");
        free(ch);
        fclose(f); 
        return NULL;
    }

    ch[len] = '\0';

    fclose(f);
    return ch;
}

int main(void){
    char *content = ouvrir_file("test.txt");

    if(content == NULL){
        fprintf(stderr, "Erreur: ouvrir_file a échoué\n");
        return 1;
    }

    printf("=== CONTENU DU FICHIER ===\n");
    printf("%s\n", content);
    printf("==========================\n");

    free(content);
    return 0;
}
