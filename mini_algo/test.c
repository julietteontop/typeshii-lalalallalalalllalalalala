#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "sound.h"
#include "wav.h"

#define L 16




int main(){
    //wav to sound
    printf("ok");
    sound_t* bon = wav_to_sound("bon.wav");
    sound_t* j = wav_to_sound("j.wav");
    sound_t* ou = wav_to_sound("ou.wav");
    sound_t* r = wav_to_sound("r.wav");
    sound_t* espace = wav_to_sound("espace.wav");
    sound_t* m = wav_to_sound("m.wav");
    sound_t* on = wav_to_sound("on.wav");
    sound_t* de =wav_to_sound("de.wav");



    //créer la track
    printf("ok");
    track_t* monde = malloc(sizeof(track_t));
    monde->n_sounds = 8;
    monde->sounds = malloc(8*sizeof(sound_t*));
    printf("ok");
    monde->sounds[0]=bon;
    monde->sounds[1]=j;
    monde->sounds[2]=ou;
    monde->sounds[3]=r;
    monde->sounds[4]=espace;
    monde->sounds[5]=m;
    monde->sounds[6]=on;
    monde->sounds[7]=de;

    printf("ok");
    //son de fin
    sound_t* fin = reduce_track(monde);
    save_sound("bonjour_monde.wav", fin);
    printf("ok");

    free_track_t(monde);
    free_sound_t(fin);


}