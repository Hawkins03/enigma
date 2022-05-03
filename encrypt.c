#include "encrypt.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


const int g_clickover[8][2] = {{23, -1}, {22, -1}, {17, -1}, {20, -1},
                               {25, -1}, {12, 18}, {0, 16}, {16, 20}};

int rotor(session_t **session_ptr, int rotor_num, char *letter_ptr) {
  if ((!session_ptr) || (!(*session_ptr)) || (!(*msg)))
    return BAD_INPUT;

  char letter = *letter_ptr;
  if ((letter <= 64) || ((letter > 90) && (letter < 97)) || (letter > 122))
    return OUT_OF_BOUNDS;

  int rotor_pos = -1;

  if (plain < 97)
    rotor_pos = plain - 65;
  else
    rotor_pos = plain - 97;

  

  strncpy(plain_ptr, &session->rotors[rotor_num % 4][rotor_pos], 1);

  click(session, rotor_num);

  return 1;
}

int click(session_t **session, int rotor_num) {
  if ((!session) || (!(*session)))
    return BAD_INPUT;

  if (rotor_num == 4) //reflector
    return 0;

  int r_pos = session->r_pos[rotor_num];
  char *rotor = session->rotors[rotor_num % 4];

  r_pos++;
  r_pos = r_pos % 26;

  if (((r_pos == g_clickover[rotor_num][0]) ||
      (r_pos == g_clickover[rotor_num][1])) && (rotor_num <= 3)) {
    click(session, rotor_num++);
  }
  return 0;
}

int output_string(char *ciphertext, int length);

int encrypt_string(char *plaintext, int length) {
  session_t *session_t = get_settings();

  char *character = NULL;
  for (int i = 0; i < length; i++) {
    strncpy(character, &plaintext[i], 1);
    if (character == "\0") {
      return -2;
    }

    for (int j = 1; i < 5; i++) {
      rotor(session, i, character);
    }
    for (int j = 3; j > 0; j--) {
      rotor(session, i, character);
    }
    strncpy(&plaintext[i], character, 1);
  }
  set_settings(session);
  free(session);
  session = NULL;

  return length;
}
