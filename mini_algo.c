#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "assert.h"
#include "stdbool.h"


typedef struct maillon{
    char *key;
    char *value;
    struct maillon* suiv;
    struct maillon* prec;
} maillon_t;

typedef struct dict{
    maillon_t* tete;
} dict_t;

/* Stocke dans *result la valeur associée à key dans d, et renvoie un booléen indiquant si 
key a été trouvée */
bool recherche (dict_t * d , char *key , char ** result ){
    for ( maillon_t * m = d -> tete ; m != NULL ; m = m -> suiv ){
        if (m -> key == key ){
            * result = m -> value ;
            return true ;
        }
     }
    return false ;
}

//compléter le dictionnaire d avec la clé key associée à la valeur value
void inserer(dict_t *d, char *key, char *value) {
    maillon_t *nouveau = (maillon_t*)malloc(sizeof(maillon_t));
    if (nouveau == NULL) return;

    nouveau->key = key;
    nouveau->value = value;
    
    nouveau->suiv = d->tete; 
    nouveau->prec = NULL;

    if (d->tete != NULL) {
        d->tete->prec = nouveau;
    }

    d->tete = nouveau;
}


//libérer la mémoire allouée par le dictionnaire
void free_dictionnaire(dict_t *d) {
    maillon_t *courant = d->tete;
    while (courant != NULL) {
        maillon_t *a_supprimer = courant;
        courant = courant->suiv;
        free(a_supprimer);
    }
    free(d);
}


int main() {
    dict_t* dico = (dict_t*)malloc(sizeof(dict_t));
    maillon_t* debut = (maillon_t*)malloc(sizeof(maillon_t));
    
    dico->tete = debut;

    inserer(dico, "b", "b.wav");
    inserer(dico, "on", "on.wav");
    inserer(dico, "j", "j.wav");
    inserer(dico, "ou", "ou.wav");
    inserer(dico, "r", "r.wav");
    inserer(dico, "m", "m.wav");
    inserer(dico, "d", "d.wav");
    inserer(dico, "e", "e.wav");


    // Test de la recherche
    char* resultat_trouve;
    if (recherche(dico, "on", &resultat_trouve)) {
        printf("Trouvé ! Valeur : %s\n", resultat_trouve);
    }

    free_dictionnaire(dico);
    return 0;
}




