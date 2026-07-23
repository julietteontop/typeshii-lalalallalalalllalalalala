#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define L 16

typedef struct sound {
    int n_samples;      // nombre d'échantillons
    int sample_rate;    // fréquence d'échantillonnage (Hz)
    int16_t* samples;   // tableau des échantillons
} sound_t;

sound_t* wav_to_sound(char* filename) {
    FILE* f = fopen(filename, "rb"); // ouverture pour lire en binaire
    if (f == NULL) {
        printf("Erreur : le fichier ne peut pas être ouvert. Vérifiez qu'il existe.\n");
        return NULL;
    }

    char chunk_id[5] = {0};
    uint32_t chunk_size = 0;
    int bits_per_sample = 0;
    int num_channels = 0;
    int sample_rate = 0;
    uint32_t data_size = 0;
    long data_offset = 0;

    // Vérification de l'en-tête RIFF/WAVE
    char riff[5] = {0}, wave[5] = {0};
    fread(riff, 1, 4, f);
    fseek(f, 4, SEEK_CUR); // on saute la taille globale du fichier
    fread(wave, 1, 4, f);
    if (strncmp(riff, "RIFF", 4) != 0 || strncmp(wave, "WAVE", 4) != 0) {
        printf("Erreur : ce fichier n'est pas un WAV valide.\n");
        fclose(f);
        return NULL;
    }

    int fmt_found = 0, data_found = 0;

    // Parcours des chunks jusqu'à trouver "fmt " puis "data"
    while (fread(chunk_id, 1, 4, f) == 4) {
        if (fread(&chunk_size, 4, 1, f) != 1) break;

        if (strncmp(chunk_id, "fmt ", 4) == 0) {
            long fmt_pos = ftell(f);
            fseek(f, fmt_pos + 2, SEEK_SET);              // NumChannels à l'offset 2
            fread(&num_channels, 2, 1, f);
            fseek(f, fmt_pos + 4, SEEK_SET);              // SampleRate à l'offset 4
            fread(&sample_rate, 4, 1, f);
            fseek(f, fmt_pos + 14, SEEK_SET);             // BitsPerSample à l'offset 14
            fread(&bits_per_sample, 2, 1, f);
            fseek(f, fmt_pos + chunk_size, SEEK_SET);     // on saute le reste du chunk
            fmt_found = 1;
        } else if (strncmp(chunk_id, "data", 4) == 0) {
            data_size = chunk_size;
            data_offset = ftell(f);
            data_found = 1;
            break; // on a ce qu'il faut, inutile de continuer
        } else {
            fseek(f, chunk_size, SEEK_CUR); // on saute les chunks inconnus (LIST, etc.)
        }
    }

    if (!fmt_found || !data_found) {
        printf("Erreur : chunk 'fmt ' ou 'data' introuvable dans le fichier.\n");
        fclose(f);
        return NULL;
    }

    if (bits_per_sample != L) {
        printf("Erreur : chaque valeur doit être encodée sur %d bits (trouvé : %d bits).\n", L, bits_per_sample);
        fclose(f);
        return NULL;
    }

    if (num_channels != 1) {
        printf("Erreur : seuls les fichiers mono sont supportés (trouvé : %d canaux).\n", num_channels);
        fclose(f);
        return NULL;
    }

    sound_t* s = malloc(sizeof(sound_t));
    if (s == NULL) {
        printf("Erreur : allocation mémoire échouée.\n");
        fclose(f);
        return NULL;
    }

    s->n_samples = data_size / (bits_per_sample / 8);
    s->sample_rate = sample_rate;
    s->samples = malloc(s->n_samples * sizeof(int16_t));
    if (s->samples == NULL) {
        printf("Erreur : allocation mémoire échouée.\n");
        free(s);
        fclose(f);
        return NULL;
    }

    fseek(f, data_offset, SEEK_SET);
    size_t lus = fread(s->samples, sizeof(int16_t), s->n_samples, f);
    if (lus != (size_t)s->n_samples) {
        printf("Attention : fichier tronqué, %zu échantillons lus sur %d attendus.\n", lus, s->n_samples);
        s->n_samples = (int)lus; // on ajuste à ce qui a réellement été lu
    }

    fclose(f);
    return s;
}

