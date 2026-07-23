#ifndef WAV_H
#define WAV_H

#define L 16
#define f_ech 44100

#include "sound.h"


//écrit un entier dans le fichier f un entier a en size octets en little-endian
void write_int(FILE* f, int a, int size);

//écrit dans le fichier f l'en-tête d'un fichier WAV de n échantillons
void write_header(FILE* f, int n);

//sauvegarde un son s dans une fichier filename
void save_sound(char* filename, sound_t* s);

//lit dans un fichier WAV filename et stocke les informations dans un sound_t, puis 
//renvoie un pointeur vers ce sound_t
sound_t* wav_to_sound(char* filename);

//à partir de 2 fichiers wav filename1 et filename2, crée un nouveau fichier wav
// filename qui est le mélange des 2 fichiers d'entrée, chacun à un volume vol1 et 
//vol2
void melange_2_wav(char* filename1, char* filename2, float vol1, float vol2, char* filename);


#endif