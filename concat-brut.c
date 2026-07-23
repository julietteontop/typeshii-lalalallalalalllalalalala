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


 