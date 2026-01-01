#include <stdio.h>


char* ouvrir_file(const char* path){
    FILE *f;
    long len_long; //NOTE: FTELL выдает всегда ЛОНГ 
    unsigned long long fread_resultat;
    f = fopen(path, "rb");
    if(f==NULL) 
        return 0;
    fseek(f, 0, SEEK_END);
    len_long = ftell(f);
    size_t len = (size_t)len_long;
    // ПРОВЕРКА ЕЛАСИ СРАБОТАЛО 
    rewind(f);

    char* ch = (char*)malloc(len+1); //ANCHOR: НИКОГДА НАХУЙ НЕ ЗАБЫВАЙ +1 ПЖЖЖЖЖЖЖЖЖЖ ДЛЯ '\0'
    // ПРОВЕРКА НА МАЛЛОК 

    fread_resultat = fread(ch, 1, len, f);
    //Ты ожидаешь len. Если меньше — ошибка → free(ch) → fclose(f) → NULL
    if(fread_resultat != len){
        //printferror
        free(ch);
        fclose(f); 
        return NULL;
    }

    ch[len] = '\0';

    fclose(f);
    return ch;
}

//TODO: fncts pour passer de file à texte code mais genre avec le bon registre sans la ponctuation et tout le reste