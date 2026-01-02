#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>



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


int normalisation_texte(char *s){
    int write = 0;
    size_t read;
    for(read=0; read<strlen(s); ++read){
        if(s[read] == '\n' || s[read] == '\t')  s[write++] = ' ';
        else if(s[read] <= 'z' && s[read] >= 'a') {
            s[write] = s[read];
            write++;}
        else if(s[read] <= 'Z' && s[read] >= 'A'){
            s[write] = tolower(s[read]);
            write++;}
        else if(s[read] == ' ') s[write++] = ' ';
    }

    if(s[write]!='\0') s[write]='\0';
    return 0;
}

int normalisation_texte_v2(char *s){
    int write = 0;
    size_t read;
    char c, prev, next;
    

    for(read=0; s[read] != '\0'; ++read){
        c = tolower((unsigned char)s[read]); //тюловер принимает только анчаир сар
        prev = (read != 0)
            ? tolower((unsigned char)s[read - 1])
            : 0;

        next = (s[read + 1] != '\0')
            ? tolower((unsigned char)s[read + 1])
            : 0;

        if(c >= 'a' && c <= 'z')
            s[write++] = c;
        

        else if(c == '-' && (next >= 'a' && next <= 'z') && (prev >= 'a' && prev <= 'z'))
            s[write++] = '-';

        else if((c == '\n' || c == '\t' || c == ' ' || c == '\'') && (write > 0 && s[write - 1] != ' '))
            s[write++] = ' ';    
        }
    
    s[write]='\0';
    return 0;
    }


void printtestcontenu(char *content){
    printf("=== CONTENU DU FICHIER ===\n");
    printf("%s\n", content);
    printf("==========================\n");
}



int main(void){
        
    char *content = ouvrir_file("test.txt");
    normalisation_texte_v2(content);

    if(content == NULL){
        fprintf(stderr, "Erreur: ouvrir_file a échoué\n");
        return 1;
    }

    printtestcontenu(content);

    free(content);
    return 0;
}
