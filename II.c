#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "my_memoire.h"

typedef struct {
    char* mot; // le mot lui-même
    size_t occurrences; // combien de fois ce mot apparaît
} Mot;

typedef struct {
    Mot* mots; // tableau dynamique de mots -> structure de structure
    size_t nb_mots; // combien de mots sont stockés actuellement
    size_t taille; // capacité actuelle du tableau
} Dico;

//Franchement, pour être franc, sans aucune retenu, j'ai connu des moments plus sympas

Dico* initDico(size_t capacite_initiale, InfoMem* mem) // BUT : crée et initialise un dictionnaire de mots
{
    // Alloue la structure Dico
    Dico* dico = myMalloc(sizeof(Dico), mem);
    if (dico == NULL) { // Si jamais l'allocation n'a pas marché
        fprintf(stderr, "échec allocation Dico\n");
        return NULL;
    }

    // Alloue le tableau de mots
    dico->mots = myMalloc(capacite_initiale * sizeof(Mot), mem);
    if (dico->mots == NULL) { // Si jamais l'allocation n'a pas marché
        fprintf(stderr, "échec allocation tableau de mots\n");
        myFree(dico, mem, sizeof(Dico)); //évite les fuites de mémoire
        return NULL;
    }

    // Initialise les compteurs
    dico->nb_mots = 0;
    dico->taille  = capacite_initiale;
    return dico;
}


void ajouterMot(Dico* dico, const char* mot, InfoMem* mem) // BUT : ajoute un mot au dictionnaire ou incrémente son compteur
{
    if (dico == NULL || mot == NULL) return; // rien à faire si paramètres invalides

    //Cherche si le mot existe déjà
    for (size_t i = 0; i < dico->nb_mots; ++i) {//Boucle pour parcourir toute la structure
        if (strcmp(dico->mots[i].mot, mot) == 0) { //Horrible vrmt mais en gros strcmp permet de comparer les chaînes de caractères par caractère. Juste c'est pas zinzin quand t'apprends son existence après 1 heure à coder une fonction qui le fait à sa place mais voilà après je dis pas que mes problèmes sont plus graves que ceux des autres mais un peu quand même. Cette expérience m'a fortement influencé mentalement et je ne crains en rien le fait de comparer l'étendue de mon traumatisme avec la pire des tortures contemporaine : la goutte d'eau c'est rien du tout à côté du sévice que m'inflige la L2 informatique
            dico->mots[i].occurrences++;
            return; // Le mot exite alors on ferme 
        }
    }

    //Redimensionnement si nécessaire
    if (dico->nb_mots == dico->taille) { //recupère les données de taille 
        size_t oldS = dico->taille;
        size_t newS = (oldS == 0) ? 1 : oldS * 2;
        size_t oldB = oldS * sizeof(Mot);
        size_t newB = newS * sizeof(Mot);


        Mot* newP = myRealloc(dico->mots, newB, mem, oldB); // On agrandi le tableau
        if (newP == NULL) {
            fprintf(stderr, "Erreur : réallocation du tableau de mots échouée\n"); //j'ai la flemme de faire un double pull oui je l'avoue face cam
            return;
        }
        dico->mots = newP;
        dico->taille = newS;
    }

    //Alloue et copie la chaîne du mot
    size_t len = strlen(mot); // Mtn que je sais crois moi que plus rien ne m'arrête
    char* copie = myMalloc(len + 1, mem);
    if (copie == NULL) {
        fprintf(stderr, "Erreur : allocation du mot échouée\n");
        return;
    }
    memcpy(copie, mot, len + 1);

    //Les paramètres + ajouter nouveau mot
    dico->mots[dico->nb_mots].mot = copie; //ajoute copie (chaîne de caractère) à la fin de la liste de mot
    dico->mots[dico->nb_mots].occurrences = 1;
    dico->nb_mots++;
}

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

int switch_fr(unsigned char c){
    switch (c) {
        case 0xA0: case 0xA2: case 0xA4: return 'a'; // à â ä
        case 0xA7: return 'c';                       // ç
        case 0xA8: case 0xA9: case 0xAA: case 0xAB: return 'e'; // è é ê ë
        case 0xAE: case 0xAF: return 'i';             // î ï
        case 0xB4: case 0xB6: return 'o';             // ô ö
        case 0xB9: case 0xBB: case 0xBC: return 'u';  // ù û ü
        default: return 0;
    }
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
        

        else if(c == '-' && (next >=     'a' && next <= 'z') && (prev >= 'a' && prev <= 'z'))
            s[write++] = '-';

        else if((unsigned char)s[read] == 0xC3 && s[read + 1] != '\0'){
            unsigned char b2 = (unsigned char)s[read + 1];
            int element_francais = switch_fr(b2);
            if(element_francais!=0){
                s[write++] = element_francais;
                read++;
            }
        }


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
