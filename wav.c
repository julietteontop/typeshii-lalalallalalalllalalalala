#include<assert.h>
#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>
#include<math.h>


#include "wav.h"

void write_int(FILE* f, int a, int size){
    while (size!=0){
        fprintf(f, "%c", a&255);
        a = a >> 8; 
        size-=1;             
    }
}

void write_header(FILE* f, int n){
    fprintf(f,"RIFF");
    write_int(f,36+L/8*n,4);
    fprintf(f,"WAVEfmt ");
    write_int(f,16,4);
    write_int(f,1,2);
    write_int(f,1,2);
    write_int(f,f_ech,4);
    write_int(f,f_ech*L/8,4);
    write_int(f,L/8,2);
    write_int(f,L,2);
    fprintf(f,"data");
    write_int(f,L*n/8,4);

}

void save_sound(char* filename, sound_t* s){
    FILE*f=fopen(filename, "wb"); 
    assert(f!=NULL);
    write_header(f,s->n_samples);
    for (int i=0; i<s->n_samples; i++){
        write_int(f,s->samples[i], L/8);
    }

    fclose(f);
}

sound_t* wav_to_sound(char* filename){ 
    FILE*f=fopen(filename, "rb"); //ouverture pour lire en binaire
    if (f==NULL){ 
        printf("Erreur : le fichier ne peut pas être ouvert. Vérifiez qu'il existe.\n");
        return 0; 
    } 
    sound_t* s=malloc(sizeof(sound_t)); 
    int l=0; 
    int L8_N=0; 
    fseek(f,34, SEEK_SET); 
    fread(&l, 2, 1, f); 
    fseek(f,40, SEEK_SET); 
    fread(&L8_N, 4, 1, f);

    s->n_samples=L8_N/(l/8);
    if (l!=L){
        printf("Erreur : chaque valeur doit être encodée sur %d bits", L);
    } 
    s->samples=malloc(s->n_samples*sizeof(int16_t)); 

    fseek(f, 44,SEEK_SET);
    for (int j=0; j<s->n_samples; j++){ 
        int16_t valeur=0; 
        fread(&valeur, sizeof(int16_t), 1, f); 
        s->samples[j]=valeur;
    }

    fclose(f);

    return s;
}

void melange_2_wav(char * filename1, char* filename2, float vol1, float vol2, char* filename){
    sound_t* s1=wav_to_sound(filename1);
    if (s1 == NULL){
        printf("Erreur : impossible de charger %s\n", filename1);
        return;
    }

    sound_t* s2=wav_to_sound(filename2);
    if (s2 == NULL){
        printf("Erreur : impossible de charger %s\n", filename2);
        free_sound_t(s1);
        return;
    }
    sound_t* s=malloc(sizeof(sound_t));

    //calcul de nb de samples du son final (maximum des 2 sons)
    if (s1->n_samples>s2->n_samples){
        s->n_samples=s1->n_samples;
    } else{
        s->n_samples=s2->n_samples;
    }

    s->samples=malloc(s->n_samples*sizeof(int16_t));

    for (int i = 0; i < s->n_samples; i++){
        float somme = 0;
        float sommeV = 0;
        
        if (i < s1->n_samples){
            somme += vol1 * s1->samples[i];
            sommeV += vol1;
        }
        
        if (i < s2->n_samples){
            somme += vol2 * s2->samples[i];
            sommeV += vol2;
        }
        
        if (sommeV != 0){
            s->samples[i] = (int16_t)(somme / sommeV);
        } else {
            s->samples[i] = 0;
        }
    }
    
    save_sound(filename, s);

    free_sound_t(s1);
    free_sound_t(s2);
    free_sound_t(s);

}





