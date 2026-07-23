#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "sound.h"

void free_sound_t(sound_t* elm){
    if (!elm){
        return;
    }
    if(elm->samples){
        free(elm->samples);
    }
    free(elm);

}

void free_track_t (track_t* elm){
    if (!elm){
        return;
    }
    if (elm->sounds){
        for (int i=0; i<elm->n_sounds; i++){
            free_sound_t(elm->sounds[i]);
        }
        free(elm->sounds);
    }
    free(elm);
}

void free_mix_t (mix_t* elm){
    for (int i=0; i<elm->n_tracks; i++){
        free_track_t(elm->tracks[i]);
    }
    free(elm->tracks);
    free(elm->vols);
    free(elm);
}

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

sound_t* reduce_mix(mix_t* m){
    sound_t* s=malloc(sizeof(sound_t));

    sound_t** tab_track=malloc(m->n_tracks*sizeof(sound_t*));
    int max=0;
    for (int i=0; i<m->n_tracks; i++){
        tab_track[i]=reduce_track(m->tracks[i]);
        if(tab_track[i]->n_samples>max){
            max=tab_track[i]->n_samples;
        }
    }

    s->n_samples=max;
    s->samples=malloc(max*sizeof(int16_t));

    for (int j=0; j<s->n_samples; j++){
        float sommeS=0;
        float sommeV=0;
        for (int k=0; k<m->n_tracks; k++){
            if(j<tab_track[k]->n_samples){
                sommeS+=m->vols[k]*tab_track[k]->samples[j];
                sommeV+=m->vols[k];
            }
        }
        if (sommeV!=0){
            s->samples[j]=(int16_t)(sommeS/(float)sommeV);
        } else {
            s->samples[j]=0;
        }
    }

    for (int l=0; l<m->n_tracks; l++){
        free_sound_t(tab_track[l]);
    }
    free(tab_track);

    return s;

}
