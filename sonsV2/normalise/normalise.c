#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <stdint.h>

#include "sound.h"

#define V_MOY 32000
//la valeur max des échantillons est 32767

/*pondere les echantillons en gros
amélioration possible : plutôt que de normaliser
par rapport au pic, le faire par rapport à la valeur efficace*/
void volume( sound_t* s){
    int16_t max = 0;
    for (int i =0; i < s->n_samples ; i++){
        if (s->samples[i]> max){
            max = s->samples[i];
        }
    }
    if (max==0){
        return;
    }

    for (int i =0; i < s->n_samples ; i++){
        s->samples[i] = s->samples[i] * V_MOY/ max ;
    }
}


//indications : pour 200ms, prendre n =8820
//pour 50ms, n = 2205 
//pour 100ms, n = 4410 
//(derrière toutes ces valeurs semblants sorties de mon génial cerveau
//se cache en réalité un calcul des plus complexe : n = fech * duree)
//on laisse couler aussi les 200 premières valeurs
//libere s aussi
sound_t* duree(sound_t* s, int n ){
    sound_t* rslt = malloc(sizeof(sound_t));
    rslt->n_samples = n;
    rslt->samples = malloc(n*sizeof(int16_t));
    for (int i = 200; i<(n+200); i++){
        if (i>=s->n_samples){
            free_sound_t(s);
            printf("le son est trop court, il ne fait pas la taille attendue");
            return rslt;
        }
        rslt->samples[i-200] = s->samples[i];
    }
    free_sound_t(s);
    return rslt;
}

//normalise
//type pour duree de la lettre 0 consonne 
// 1 voyelle courte 2 voyelle longue
sound_t* normalise(sound_t* s , int type){
    int n =0;
    if (type==0) {n = 2205;}
    if (type==1) { n = 4410;}
    if (type==2) { n = 8820;}
    sound_t* rslt = duree(s , n);
    volume(rslt);
    return rslt;
}

int main(){
    return 0;
}
