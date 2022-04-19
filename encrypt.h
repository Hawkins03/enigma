#include "init.h"

#define N_ROTORS (4)

#define NULL_PTR        (-1)
#define OUT_OF_BOUNDS   (-2)

int rotor(settings_struct_t *settings, int rotor, char *plain_ptr);
int click(settings_struct_t *settings, int rotor_num);
