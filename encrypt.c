#include "encrypt.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/*
 * takes in a session, the rotor num (0-4) and a pointer to the first letter
 * in a string (the string can be size 1).
 *
 * If the letter isn't either a capital letter or a lowercase letter, it
 * returns OUT_OF_BOUNDS (-2). Otherwise if the pointers turn out to be null,
 * it returns BAD_INPUT (-1). On a success it returns SUCCESS (1).
 */

char rotor(const session_t *sesh, int rotor_num, char *letter_ptr) {
  printf("0.\n");
  // nothing points to null right?
  if ((!sesh) || (!(*letter_ptr)))
    return BAD_INPUT;

  printf("1.\n");

  // the letter is within bounds right?
  char letter = *letter_ptr;
  if ((letter <= 64) || ((letter > 90) && (letter < 97)) || (letter > 122))
    return OUT_OF_BOUNDS;

  printf("2.\n");

  // finding where the letter is in the alphabet.
  int rotor_pos = -1;
  if (letter < 97)
    rotor_pos = letter - 65;
  else
    rotor_pos = letter - 97;

  if ((rotor_pos > 26) || (rotor_pos < 0))
    printf("rotor_pos is %d", rotor_pos);

  printf("3. %d\n", sesh->rotors[rotor_num % 4][rotor_pos % 26]);

  // replacing the letter with an encrypted letter. (DOUBLE CHECK)
  return sesh->rotors[rotor_num % 4][rotor_pos % 26];
} /* rotor() */

/*
 * takes in a pointer to a session, and a rotor number. This isn't to be called
 * by the user, because it's always called once per letter by encrypt_string.
 *
 * once per letter, the rotors need to rotate. This first rotates the 1st
 * rotor once, and if that rotor is at the correct position to be rotated
 * again, then the function calls itself to rotate the next rotor.
 */

int click(session_t **session_ptr, int rotor_num) {
  const int turnover[8][2] = {{23, -1}, {22, -1}, {17, -1}, {20, -1},
                               {25, -1}, {12, 18}, {0, 16}, {16, 20}};

  //checking for null values
  if ((!session_ptr) || (!(*session_ptr)))
    return BAD_INPUT;

  // kills the recursive loop (DO NOT TOUCH)
  if ((rotor_num >= 4) || (rotor_num < 0))
    return 0;

  // var setup
  session_t *sesh = *session_ptr;

  // rotating the rotor.
  sesh->r_pos[rotor_num] = (sesh->r_pos[rotor_num] + 1) % 26;

  // checking to see if I need to rotate the next rotor
  if ((sesh->r_pos[rotor_num] == turnover[rotor_num][0]) ||
      (sesh->r_pos[rotor_num] == turnover[rotor_num][1]))
    click(session_ptr, rotor_num++);

  *session_ptr = sesh;
  return SUCCESS;
} /* click() */

/*
 * runs through a string encrypting letters (note, sets all letters to
 * lowercase (to be fixed)). Returns an error if plaintext points to NULL.
 *
 * Note, string must be a pointer, not array (so that it can be rewritten).
 */

int encrypt_string(char *plaintext) {
  if ((!plaintext) || (strlen(plaintext) <= 0))
    return BAD_INPUT;

  session_t *sesh = get_settings();
  printf("plaintext = %s\n", plaintext);

  fflush(NULL);
  int i = 0;
  while (plaintext[i] != 0) {
    printf("%d<%c>\n", i, plaintext[i]);

    for (int j = 0; j < 4; j++) {
      plaintext[i] = rotor(sesh, j, &plaintext[i]);
      printf("after\n");
      printf("%c\n", plaintext[i]);
    }

    for (int j = 2; j >= 0; j--) {
      rotor(sesh, j, &plaintext[i]);
      printf("%c\n", plaintext[i]);
    }

    click(&sesh, 0);
    i++;
  }
  set_settings(sesh);
  // kill sesh

  return i;
} /* encrypt_string() */
