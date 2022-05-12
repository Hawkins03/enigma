#include "init.h"

#ifndef ENCRYPT_H
#define ENCRYPT_H

#define N_ROTORS (4)

#define BAD_INPUT       (-1)
#define OUT_OF_BOUNDS   (-2)

char rotor(const session_t *sesh, int rotor_num, char letter);
char plugboard(const session_t *sesh, char letter);
int output_string(char *ciphertext, int length);
int click(session_t **session, int rotor_num);
int encrypt_string(char **plaintext);
char encrypt_letter(session_t *sesh, char letter);

#endif /* ENCRYPT_H */
