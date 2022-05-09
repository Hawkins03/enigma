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
  // nothing points to null right?
  if ((!sesh) || (!(*letter_ptr)))
    return 0;

  // the letter is within bounds right?
  char letter = *letter_ptr;
  if ((letter <= 64) || ((letter > 90) && (letter < 97)) || (letter > 122))
    return 0;

  // finding where the letter is in the alphabet.
  int rotor_pos = -1;
  if (letter < 97)
    rotor_pos = letter - 65;
  else
    rotor_pos = letter - 97;

  // replacing the letter with an encrypted letter. (DOUBLE CHECK)
  return sesh->rotors[rotor_num % 4][(rotor_pos + sesh->r_set[rotor_num]) % 26];
} /* rotor() */

/*
 * takes in a pointer to a session, and a rotor number. This isn't to be called
 * by the user, because it's always called once per letter by encrypt_string.
 *
 * once per letter, the rotors need to rotate. This first rotates the 1st
 * rotor once, and if that rotor is at the correct position to be rotated
 * again, then the function calls itself to rotate the next rotor.
 */

int click(session_t **sesh_ptr, int rotor_num) {
  printf("click! (%d)", (*sesh_ptr)->r_set[rotor_num]);
  const int turnover[8][2] = {{23, -1}, {22, -1}, {17, -1}, {20, -1},
                               {25, -1}, {12, 18}, {0, 16}, {16, 20}};

  //checking for null values
  if ((!sesh_ptr) || (!(*sesh_ptr)))
    return BAD_INPUT;

  // kills the recursive loop (DO NOT TOUCH)
  if ((rotor_num >= 4) || (rotor_num < 0))
    return 0;

  int rotor_selection = (*sesh_ptr)->r_pos[rotor_num];
  printf("%d = %d", rotor_selection, turnover[rotor_selection][0]);


  // rotating the rotor.
  (*sesh_ptr)->r_set[rotor_num] = ((*sesh_ptr)->r_set[rotor_num] + 1) % 26;

  // checking to see if I need to rotate the next rotor
  if (((*sesh_ptr)->r_set[rotor_num] == turnover[rotor_selection][0]) ||
      ((*sesh_ptr)->r_set[rotor_num] == turnover[rotor_selection][1])) {
    printf("activating");
    click(sesh_ptr, ++rotor_num);
    printf("\n");
    print_settings(*sesh_ptr);
  }

  return SUCCESS;
} /* click() */

/*
 * runs through a string encrypting letters (note, sets all letters to
 * lowercase (to be fixed)). Returns an error if plaintext points to NULL.
 *
 * Note, string must be a pointer, not array (so that it can be rewritten).
 */

int encrypt_string(char **plaintext_ptr) {
  if ((!plaintext_ptr) || (!*plaintext_ptr) || (strlen(*plaintext_ptr) <= 0))
    return BAD_INPUT;

  char *plaintext = *plaintext_ptr;

  session_t *sesh = get_settings();

  int i = 0;
  while (plaintext[i] != 0) {
    printf("%d<%c> ", i, plaintext[i]);
    for (int j = 0; j < 4; j++) {
      char letter = rotor(sesh, j, &plaintext[i]);
      if (letter != 0)
        plaintext[i] = letter;
      printf("%d%c ", j, plaintext[i]);
    }

    for (int j = 2; j >= 0; j--) {
      char letter = rotor(sesh, j, &plaintext[i]);
      if (letter != 0)
        plaintext[i] = letter;
      printf("%d%c ", j, plaintext[i]);
    }

    i++;
    printf("(%ld)", strlen(plaintext));
    printf("click was : %d", click(&sesh, 0));
    printf("\n");
  }

  print_settings(sesh);
  printf("set was %s\n", set_settings(sesh)? "good": "bad");
  close_session(&sesh, NULL);
  return i;
} /* encrypt_string() */
