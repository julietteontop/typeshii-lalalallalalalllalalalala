#include<assert.h>
#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>
#include<math.h>


typedef struct sound {
    int n_samples ; // nombre d' ́e chantillons
    int16_t * samples ; // tableau des  ́e chantillons
} sound_t ;

typedef struct track {
 int n_sounds ; // nombre de sons dans la piste
 sound_t ** sounds ; // liste des sons
 } track_t ;


//concatène tous les sons du track pour en faire un unique sound --> concaténation brute
sound_t* reduce_track(track_t* t){
    sound_t* s=malloc(sizeof(sound_t));

    s->n_samples=0;
    for(int k=0; k<t->n_sounds; k++){
        s->n_samples+=t->sounds[k]->n_samples;
    }

    s->samples=malloc(s->n_samples*sizeof(int16_t));
 
    int indice = 0;
    for (int i = 0; i < t->n_sounds; i++) {
        sound_t* courant = t->sounds[i];
        for (int j = 0; j < courant->n_samples; j++) {
            s->samples[indice] = courant->samples[j];
            indice++;
        }
    }

    return s;

}