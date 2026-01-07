//* les included:

#include "projet.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
    

//*fonctions 

//? dicos:

//!1:
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

//!2:
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

//!3: 
//le freedico??

void freeDico(Dico* dico, InfoMem* mem) // BUT : libère entièrement un dictionnaire
{
    if (dico == NULL) return; // rien à faire si le dictionnaire est NULL

    // Libère chaque mot
    for (size_t i = 0; i < dico->nb_mots; ++i) {
        if (dico->mots[i].mot != NULL) {
            myFree(dico->mots[i].mot, mem, strlen(dico->mots[i].mot) + 1);
        }
    }

    // Libère le tableau de mots
    if (dico->mots != NULL) {
        myFree(dico->mots, mem, dico->taille * sizeof(Mot));
    }

    // Libère la structure Dico
    myFree(dico, mem, sizeof(Dico));
}


// fonction pour algo 1 :
void ajouterMotTexteADico(char* texte, Dico* dico, InfoMem* mem) // BUT : découpe le texte en mots et les ajoute au dictionnaire
{
    if (texte == NULL || dico == NULL) return;

    size_t i = 0;
    while (texte[i] != '\0') { // tant qu'on n'a pas atteint la fin du texte

        while (texte[i] == ' ') // ignore tous les espaces
            i++;

        if (texte[i] == '\0') // si jamais on tombe sur la fin du texte après les espaces
            break;

        size_t start = i; // on mémorise l'index où commence le mot
        while (texte[i] != ' ' && texte[i] != '\0') // on avance jusqu'à la fin du mot (prochain espace ou fin de texte)
            i++;

        size_t len = i - start;
        char* mot = myMalloc(len + 1, mem); // alloue temporairement le mot + '\0'
        if (mot == NULL)
            return;

        memcpy(mot, &texte[start], len); // copie les caractères du mot
        mot[len] = '\0'; // termine la chaîne correctement
        ajouterMot(dico, mot, mem); // incrémente compteur si existant ou ajoute le nouveau mot
        myFree(mot, mem, len + 1); // évite fuite mémoire
    }
}

// LETTSSS GO ALGO 2 :

int comparerMots(const void *mot1, const void *mot2) // BUT : Déterminer l’ordre entre deux mots en fonction de leur nombre d’occurrences 
{
    const Mot *m1 = (const Mot *)mot1; // const -> la fonction ne doit pas modifier les données
    const Mot *m2 = (const Mot *)mot2;

    // On trie d'abord par nombre d'occurrences (décroissant)
    if (m1->occurrences < m2->occurrences)
        return 1;
    if (m1->occurrences > m2->occurrences)
        return -1;

    // Si égalité -> on trie par ordre alphabétique
    return strcmp(m1->mot, m2->mot);
}

void trierDicoParOccurences(Dico* dico) // BUT : Trier le dictionnaire de mots par nombre d’occurrences décroissant
{
    if (dico == NULL || dico->nb_mots == 0)
        return;

    qsort(
        dico->mots, // le tableau à trier
        dico->nb_mots, // combien d'éléments
        sizeof(Mot), // taille d'un élément
        comparerMots // La règle de comparaison
    );
}

//?ALGO4:

Noeud* creer_noeud(const char * mot, InfoMem* infoMem){

    if (!mot) return NULL; //только ли нулль?

    Noeud * nouv_noeud = (Noeud*)myMalloc(sizeof(Noeud), infoMem);
    if(!nouv_noeud){
        fprintf(stderr, "erreur: malloc n'a pas réussi à allouer de la mémoire!");
        return NULL;
    }

    char* copy_mot = (char*)myMalloc(sizeof(char) * (strlen(mot)+1), infoMem);
    if(!copy_mot){
        fprintf(stderr, "erreur: malloc num2 n'a pas réussi à allouer de la mémoire!");
        myFree(nouv_noeud, infoMem, sizeof(Noeud));
        return NULL;
    }

    strcpy(copy_mot, mot);

    nouv_noeud -> data.mot = copy_mot;//ok
    nouv_noeud -> data.occurrences = 1;
    nouv_noeud -> gauche = NULL;
    nouv_noeud -> droite = NULL;
    return nouv_noeud;
}

Noeud* insert_ou_incrementer(Arbre*arbre, const char* mot, InfoMem* infoMem){
    if(!arbre || !mot) return NULL;

    if(!arbre->racine){
        arbre->racine = creer_noeud(mot, infoMem);
        if(!arbre->racine) return NULL;
        arbre->nb_mots_uniques++;
        return arbre->racine;
    }
    Noeud* cur = arbre->racine;

    while(1){//не 0 иначе выкл
        int comp = strcmp(mot, cur->data.mot); //обьявление compa в обязаловку
        if (comp == 0){
            cur->data.occurrences++;
            return cur;
        }
        else if(comp < 0){ //genre si mot < arbre mot
            if (!cur->gauche){
                cur->gauche = creer_noeud(mot, infoMem);
                if(!cur->gauche) return NULL;
                arbre->nb_mots_uniques++;
                return cur->gauche;
            }
            else cur = cur->gauche;
        }
        else{
            if (!cur->droite){
                cur->droite = creer_noeud(mot, infoMem);
                if(!cur->droite) return NULL;
                arbre->nb_mots_uniques++;
                return cur->droite;
            }
            else cur = cur->droite;
        }
    }
    
}

void liberer_arbre(Noeud *noeud, InfoMem *mem)
{
    if (!noeud || !mem) return;

    liberer_arbre(noeud->gauche, mem);
    liberer_arbre(noeud->droite, mem);

    if (noeud->data.mot) {
        size_t len = strlen(noeud->data.mot) + 1;
        myFree(noeud->data.mot, mem, len);
        noeud->data.mot = NULL;
    }

    myFree(noeud, mem, sizeof(Noeud));
}

// Tout ce qui touche à la gestion des options dans le main : 

void print_help(char *prog) { // BUT : Affiche les aides tout simplement
    printf("\nUsage: %s [-n int] [-a algo1|algo2|algo3|algo4] [--help] [-s sortie.txt] [-l perf.csv] fichiers...\n", prog);
    printf("\n  -n int        Nombre de mots à afficher (top N). Default: 20\n");
    printf("  -a algoX      Choix de l'algorithme : algo1, algo2, algo3, algo4. Default: algo1\n");
    printf("  -s fichier    Ecrire les résultats (mot occurrences) dans ce fichier (append).\n");
    printf("  -l fichier    Ecrire les performances (CSV) dans ce fichier (append).\n");
    printf("  --help        Affiche cette aide.\n");
}

int afficher_resultats(Dico *dico, size_t nb_max_mots, const char *fichier_sortie, const char *nom_fichier_entree) // BUT : Afficher les top N mots les plus fréquents, soit à l’écran soit dans un fichier
{
    if (dico == NULL) return -1;

    FILE *sortie = stdout;   // par défaut -> affichage à l'écran
    int fermer_fichier = 0;  // est-ce qu'on devra fermer le fichier à la fin ?
    (void)nom_fichier_entree;

    // Si un fichier de sortie est demandé
    if (fichier_sortie != NULL) {
        sortie = fopen(fichier_sortie, "a"); // append
        if (sortie == NULL) {
            perror("fopen sortie");
            return -1;
        }
        fermer_fichier = 1;
    }

    //fprintf(sortie, "--- RESULTATS pour %s ---\n", nom_fichier_entree ? nom_fichier_entree : ""); // Titre des résultats

    size_t nb_a_afficher = (nb_max_mots == 0 || nb_max_mots > dico->nb_mots) ? dico->nb_mots : nb_max_mots;  // Combien de mots on affiche réellement

    for (size_t i = 0; i < nb_a_afficher; ++i) { // Affichage des mots
        fprintf(sortie, "%s %zu\n", dico->mots[i].mot, dico->mots[i].occurrences);
    }
    //fprintf(sortie, "\n");

    if (fermer_fichier) // On ferme le fichier seulement si on l'a ouvert nous-même
        fclose(sortie);

    return 0;
}

int ecrire_perf_csv(
    const char *chemin_perf, // chemin du fichier CSV
    const char *nom_algo, // "algo1", "algo2", ...
    const char *nom_fichier, // fichier texte analysé
    size_t nb_mots_total, // nombre total de mots (tokens)
    size_t nb_mots_distincts, // nombre de mots différents
    double temps_exec, // temps d'exécution en secondes
    size_t mem_allouee, // cumul mémoire allouée
    size_t mem_desallouee, // cumul mémoire libérée
    size_t pic_memoire // pic mémoire utilisé
)
{
    if (chemin_perf == NULL) return -1; // pas de fichier -> stop

    int fichier_nouveau = 0; // Vérifie si le fichier existe déjà
    FILE *test = fopen(chemin_perf, "r");
    if (test == NULL)
        fichier_nouveau = 1;
    else
        fclose(test);

    FILE *f = fopen(chemin_perf, "a"); // Ouvre le fichier en ajout
    if (f == NULL) {
        perror("fopen perf");
        return -1;
    }

    if (fichier_nouveau) { // Écrit l'en-tête si le fichier est nouveau
        fprintf(f, "algo,inputfile,total_words,distinct_words,"
                   "time_s,cumul_alloc,cumul_desalloc,max_alloc\n");
    }

    // Écrit une ligne de données
     fprintf(f, "%s,%s,%zu,%zu,%.6f,%zu,%zu,%zu\n",
            nom_algo ? nom_algo : "", // si nom_algo NULL, écrit chaîne vide
            nom_fichier ? nom_fichier : "", // idem pour le nom du fichier
            nb_mots_total, // nombre total de mots dans le texte
            nb_mots_distincts, // nombre de mots uniques
            temps_exec, // durée d'exécution
            mem_allouee, // cumul des allocations
            mem_desallouee, // cumul des désallocations
            pic_memoire); // pic mémoire observé

    fclose(f); // ferme le fichier proprement
    return 0;
}



size_t compter_mots(const char *texte) // BUT : compte le nombre de mots dans un texte
{

    if (texte == NULL) return 0;

    size_t pos = 0;      // position actuelle dans la chaîne de caractères
    size_t nb_mots = 0;  // compteur de mots

    while (texte[pos] != '\0') { // Parcours toute la chaîne jusqu'au caractère de fin '\0'
        while (texte[pos] == ' ') pos++;

        if (texte[pos] == '\0') break;

        while (texte[pos] != ' ' && texte[pos] != '\0') pos++;
        nb_mots++;
    }
    return nb_mots;
}





//? le fichier:

//!1:
char* ouvrir_file(const char* path, InfoMem* mem, size_t* out_len){
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

    char* ch = (char*)myMalloc(len + 1, mem); //ANCHOR: НИКОГДА НАХУЙ НЕ ЗАБЫВАЙ +1 ПЖЖЖЖЖЖЖЖЖЖ ДЛЯ '\0'
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
        myFree(ch, mem, len + 1);
        fclose(f); 
        return NULL;
    }

    ch[len] = '\0';
    fclose(f);
    if (out_len != NULL) {
        *out_len = len + 1;
    }
    return ch;
}

//? normalisation:

//!1:
//надо подумать поменять на статик эту хуцню
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

//!2:
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
