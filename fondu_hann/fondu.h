#ifndef FONDU_H
#define FONDU_H

sound_t** separe_son (sound_t* s1, sound_t* s2, int id);
sound_t* fenetre_hann(sound_t* fin, sound_t* deb , int fe);
sound_t* fondu(sound_t* s1, sound_t* s2, int fe, int id);

#endif

