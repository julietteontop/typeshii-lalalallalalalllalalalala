#ifndef SOUND_H
#define SOUND_H


typedef struct sound{
    int n_samples; //nb d'échantillons
    int16_t* samples; //tableau des échantillons
} sound_t ;


typedef struct track{
    int n_sounds; //nombre de sons dans la piste
    sound_t** sounds; //liste des sons
} track_t;


typedef struct mix{
    int n_tracks; //nombre de pistes
    track_t** tracks; //liste des pistes
    float* vols; //liste des volumes des pistes
} mix_t; 


//libère la mémoire allouée pour un élément elm de type sound_t
void free_sound_t(sound_t* elm);


//libère la mémoire allouée pour un élément elm de type track_t
void free_track_t (track_t* elm);


//libère la mémoire allouée pour un élément de type mix_t
void free_mix_t(mix_t* elm);

//renvoie un son composé de tous les sons de la piste t mis bout à bout
sound_t* reduce_track(track_t* t);

//renvoie un son composé de la moyenne des sons de chaque piste d'un mix m, chaque
//piste étant pondérée par un volume
sound_t* reduce_mix(mix_t* m);


#endif

