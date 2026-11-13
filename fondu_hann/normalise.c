#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <stdint.h>

#include "sound.h"
#include "wav.h"

#define V_MOY 32000
#define N_120 5292
#define N_100 4410
#define N_50 2205
//la valeur max des échantillons est 32767

/*pondere les echantillons en gros
amélioration possible : plutôt que de normaliser
par rapport au pic, le faire par rapport à la valeur efficace*/
void volume( sound_t* s){
    int16_t max = 0;
    for (int i =0; i < s->n_samples ; i++){
        int a = abs(s->samples[i]);
        if (a > max){
            max = a;
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


// à partir d'un son s et d'une valeur de n précalculée, coupe les 200 premières valeurs de s et renvoie un son de n échantillons
// à la fin, libère s. si le son s est trop court, on complète les valeurs manquantes par des 0
sound_t* duree(sound_t* s, int n ){
    sound_t* rslt = malloc(sizeof(sound_t));
    rslt->n_samples = n;
    rslt->samples = malloc(n*sizeof(int16_t));
    for (int j = 0; j < n; j++){
        rslt->samples[j] = 0;
    }
    for (int i = 200; i<(n+200); i++){
        if (i >= s->n_samples){
            free_sound_t(s);
            printf("le son est trop court, il ne fait pas la taille attendue");
            return rslt;
        }
        rslt->samples[i-200] = s->samples[i];
    }
    free_sound_t(s);
    return rslt;
}

/*filtre passe bas de fc=4000Hz (c'est bien pour la voix askip)*/
sound_t* passe_bas(sound_t* s){
    float fc = 4000.0;
    float fs = 44100.0;
    float alpha = (2*3.1415*fc)/(2*3.1415*fc + fs);
    int n = s->n_samples;

    sound_t* rslt = malloc(sizeof(sound_t));
    rslt->n_samples = n;
    rslt->samples = malloc(n*sizeof(int16_t));
    rslt->samples[0]=0;
    
    for (int i = 1 ; i<n ; i++){
        rslt->samples[i] = alpha*s->samples[i] + (1-alpha)*rslt->samples[i-1];
    }
    free_sound_t(s);
    return rslt;
}

//normalise
//type pour duree de la lettre 0 consonne plosive (b, t, p, k..)
// 1 consonne fricative (s, ch, f..) 2 voyelle
sound_t* normalise(sound_t* s , int type){
    int n =0;
    sound_t* rslt;
    if (type==0) {n = N_50;}
    if (type==1) { n = N_100;}
    if (type==2) { 
        n = N_120;
        rslt = passe_bas(s);
    } else {
        rslt = s;
    }
    sound_t* rslt2 = duree(rslt , n);
    volume(rslt2);


    return rslt2;
}

int main(){
    sound_t* a = wav_to_sound("a.wav");
    save_sound("a_pas_normalise.wav", a);

    sound_t* a1 = normalise(a, 2);
    save_sound("a_normalise.wav", a1);

    free_sound_t(a1);


    return 0;


 }

