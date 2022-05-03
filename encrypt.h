#include "init.h"

#define N_ROTORS (4)

#define BAD_INPUT       (-1)
#define OUT_OF_BOUNDS   (-2)

int rotor(settings_struct_t *settings, int rotor, char *plain_ptr);
int output_string(char *ciphertext, int length);
int click(settings_struct_t *settings, int rotor_num);
