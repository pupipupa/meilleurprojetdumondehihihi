//*: les structures
typedef struct {
    char* mot; // le mot lui-même
    size_t occurrences; // combien de fois ce mot apparaît
} Mot;

typedef struct {
    Mot* mots; // tableau dynamique de mots -> structure de structure
    size_t nb_mots; // combien de mots sont stockés actuellement
    size_t taille; // capacité actuelle du tableau
} Dico;

//*: les prototypes

Dico* initDico(size_t capacite_initiale, InfoMem* mem);
void ajouterMot(Dico* dico, const char* mot, InfoMem* mem);

//void libererDico(Dico* dico, InfoMem* mem); 
//!il faut l'ajouter sinon y'a trop de pertes de mémoire je crois