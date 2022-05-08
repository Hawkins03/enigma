#include "init.h"

#ifndef ENCRYPT_H
#define ENCRYPT_H

#define N_ROTORS (4)

#define BAD_INPUT       (-1)
#define OUT_OF_BOUNDS   (-2)
#define SUCCESS         (1)

char rotor(const session_t *sesh, int rotor_num, char *letter_ptr);
int output_string(char *ciphertext, int length);
int click(session_t **session, int rotor_num);
int encrypt_string(char *plaintext);

#endif /* ENCRYPT_H */
