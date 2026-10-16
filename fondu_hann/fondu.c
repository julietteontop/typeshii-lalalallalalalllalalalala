#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <assert.h>
#include <math.h>
#include "wav.h"
#include "sound.h"

#define FE 44100
#define TEMPS_CONS 0.05
#define TEMPS_VOY 0.02
#define TEMPS_ENTRE_SYL 0.01

//Renvoie un tableau contenant débuts1, fins1, debuts2, fins2 où finS1 et début s2 ont nb_ech sample
//id correspond au nombre de samples de la séparation, ie la longueur du fondu
sound_t** separe_son (sound_t* s1, sound_t* s2, int id){

    assert(id==0 || id==1 || id==2);
    int nb_ech =0;

    if (id == 0) {
        nb_ech =FE * TEMPS_CONS;
    }


    if (id == 1) {
        nb_ech =FE * TEMPS_VOY;
    }


    if (id == 2) {
        nb_ech =FE * TEMPS_ENTRE_SYL;
    }

    int n1 = s1->n_samples ;
    int n2 = s2-> n_samples; 
    
    sound_t* debs1 = malloc(sizeof(sound_t));
    debs1-> n_samples = n1 - nb_ech;
    debs1->samples = malloc((n1 -nb_ech)* sizeof(int16_t));

    sound_t* fins1 = malloc(sizeof(sound_t));
    fins1-> n_samples = nb_ech;
    fins1->samples = malloc((nb_ech)* sizeof(int16_t));

    sound_t* debs2 = malloc(sizeof(sound_t));
    debs2-> n_samples = nb_ech;
    debs2->samples = malloc((nb_ech)* sizeof(int16_t));

    sound_t* fins2 = malloc(sizeof(sound_t));
    fins2-> n_samples = n2 - nb_ech;
    fins2->samples = malloc((n2 -nb_ech)* sizeof(int16_t));

    sound_t** res = malloc(sizeof(sound_t*)*4);
    res[0]=debs1;
    res[1]= fins1;
    res[2] = debs2;
    res[3] = fins2;


    for (int i = n1-1 ; i >-1 ; i--){
        if (i< (n1 -nb_ech)) {
            debs1->samples [i] = s1->samples[i];
        }
        else {
            fins1->samples [i -(n1-nb_ech)] = s1->samples[i];
        }
    }

    for (int i = 0 ; i < n2 ; i++){
        if (i< (nb_ech)) {
            debs2->samples [i] = s2->samples[i];
        }
        else {
            fins2->samples [i - nb_ech] = s2->samples[i];
        }
    }
    printf("separe son ok \n");
    return res;

}





//superpose les sons fin et début avec une pondération (fenêtre de Hann) (voir formle doc)
//les sons pris sont deja les sons coupés
sound_t* fenetre_hann(sound_t* fin, sound_t* deb , int fe){
    int n =fin->n_samples;
    assert(fin->n_samples == deb->n_samples);
    sound_t* res = malloc(sizeof(sound_t));
    res->n_samples = n;
    res->samples =  malloc(n* sizeof(int16_t));
    float duree = (float)n/fe;

    for(int i= 0; i<n ; i++){ //faut t = i/ durée du son, 
        float t = (float)i*duree/n;
        float debut_son = 0.5*(1 -cos(M_PI*(float)t/duree));
        float fin_son = 1 - debut_son;
        res->samples[i] = fin_son*fin->samples[i] + debut_son*deb->samples[i];
        
    }
    printf("hann ok \n");
    return res;



}

//sépare, fond, et reconcatène les deux sons d'entrée. Renvoie un sound
sound_t* fondu(sound_t* s1, sound_t* s2, int fe, int id){
    sound_t** tab = separe_son (s1, s2, id);
    sound_t* fond = fenetre_hann(tab[1], tab[2], fe);
    track_t* tr = malloc(sizeof(track_t));
    
    tr->n_sounds = 3;
    tr->sounds = malloc(3*sizeof(sound_t*));
    tr->sounds[0] = tab[0];
    tr->sounds[1]=fond;
    tr->sounds[2]=tab[3];
    printf("track ok \n");
    sound_t* res = reduce_track (tr);

    // for(int i =0; i<4; i++){
    //     free_sound_t (tab[i]);
    // }
    // free_sound_t (fond); 
    // free_track_t (tr);
    printf("fondu ok \n");
    return res ; 

}


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



    
    printf("debut fondus ok \n");

    sound_t* n1 = fondu(bon , j , FE, 2);
    sound_t* n2 = fondu(n1 , ou , FE, 0);
    sound_t* n3 = fondu(n2 , r , FE, 1);
    sound_t* n4 = fondu(n3 , espace , FE, 2);
    sound_t* n5 = fondu(n4 , m , FE, 2);
    sound_t* n6 = fondu(n5 , on , FE, 0);
    sound_t* n7 = fondu(n6 , de , FE, 2);
    sound_t* n8 = fondu(n7 , espace , FE, 2);
    sound_t* n9 = fondu(n8 , r , FE, 2);
    sound_t* n10 = fondu(n9 , on , FE, 0);
    sound_t* n11 = fondu(n10 , j , FE, 1);
    sound_t* n12 = fondu(n11 , on , FE, 0);
    sound_t* fin = fondu(n12 , r , FE, 1);

    printf("fin des fondus ok \n");

    save_sound("bjr_monde_ronjon.wav", fin);
    printf("ok");


    free_sound_t(fin);



    return 0;
}