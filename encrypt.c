#include "encrypt.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


const int g_clickover[8][2] = {{23, -1}, {22, -1}, {17, -1}, {20, -1},
                               {25, -1}, {12, 18}, {0, 16}, {16, 20}};

int rotor(settings_struct_t *settings, int rotor_num, char *plain_ptr) {
  if (!plain_ptr) {
    return NULL_PTR;
  }

  char plain = *plain_ptr;

  if ((plain < 65) || ((plain > 90) && (plain < 97)) || (plain > 122)) {
    return OUT_OF_BOUNDS;
  }

  int rotor_pos = -1;

  if (plain < 97) { 
    rotor_pos = plain - 65;
  }
  else {
    rotor_pos = plain - 97;
  }

  strncpy(plain_ptr, &settings->rotors[rotor_num % 4][rotor_pos], 1);

  click(settings, rotor_num);

  return 1;
}

int click(settings_struct_t *settings, int rotor_num) {
  if (rotor_num == 4) { //reflector
    return 0;
  }

  int r_pos = settings->r_pos[rotor_num];
  char *rotor = settings->rotors[rotor_num % 4];

  r_pos++;
  r_pos = r_pos % 26;

  if (((r_pos == g_clickover[rotor_num][0]) ||
      (r_pos == g_clickover[rotor_num][1])) && (rotor_num <= 3)) {
    click(settings, rotor_num++);
  }
  return 0;
}

int output_string(char *ciphertext, int length);

int encrypt_string(char *plaintext, int length) {
  settings_struct_t *settings = get_settings();

  char *character = NULL;
  for (int i = 0; i < length; i++) {
    strncpy(character, &plaintext[i], 1);
    if (character == "\0") {
      return -2;
    }

    for (int j = 1; i < 5; i++) {
      rotor(settings, i, character);
    }
    for (int j = 3; j > 0; j--) {
      rotor(settings, i, character);
    }
    strncpy(&plaintext[i], character, 1);
  }
  set_settings(settings);
  free(settings);
  settings = NULL;

  return length;
}
