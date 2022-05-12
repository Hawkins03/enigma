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
  if ((!sesh) || (!letter_ptr))
    return 0;

  // the letter is within bounds right?
  char letter = *letter_ptr;

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
 * takes a character and if it matches a character on the plugboard, the
 * function returns the complimentary character back. (Not for personal use.)
 */

char plugboard(const session_t *sesh, char *letter_ptr) {
  if ((!sesh ) || !(letter_ptr))
    return 0;

  char letter = *letter_ptr;

  //checking for capital letters.
  if (letter < 97)
    letter += 32;

  for (int i = 0; i < 10; i++) {
    if (letter == sesh->plug_top[i])
      return sesh->plug_bot[i];
    if (letter == sesh->plug_bot[i])
      return sesh->plug_top[i];
  }
  return letter;
} /* plugboard() */

/*
 * takes in a pointer to a session, and a rotor number. This isn't to be called
 * by the user, because it's always called once per letter by encrypt_string.
 *
 * once per letter, the rotors need to rotate. This first rotates the 1st
 * rotor once, and if that rotor is at the correct position to be rotated
 * again, then the function calls itself to rotate the next rotor.
 */

int click(session_t **sesh_ptr, int rotor_num) {
  const int turnover[8][2] = {{23, -1}, {22, -1}, {17, -1}, {20, -1},
                               {25, -1}, {12, 18}, {0, 16}, {16, 20}};

  //checking for null values
  if ((!sesh_ptr) || (!(*sesh_ptr)))
    return BAD_INPUT;

  // kills the recursive loop (DO NOT TOUCH)
  if ((rotor_num >= 4) || (rotor_num < 0))
    return 0;

  int rotor_selection = (*sesh_ptr)->r_pos[rotor_num];


  // rotating the rotor.
  (*sesh_ptr)->r_set[rotor_num] = ((*sesh_ptr)->r_set[rotor_num] + 1) % 26;

  // checking to see if I need to rotate the next rotor
  if (((*sesh_ptr)->r_set[rotor_num] == turnover[rotor_selection][0]) ||
      ((*sesh_ptr)->r_set[rotor_num] == turnover[rotor_selection][1]))
    click(sesh_ptr, ++rotor_num);

  return SUCCESS;
} /* click() */

/*
 * runs through a string encrypting letters (note, sets all letters to
 * lowercase (to be fixed)). Returns an error if plaintext points to NULL.
 *
 * Note, string must be a pointer, not array (so that it can be rewritten).
 *
 * also, if you want a detailed description of what's going in behind the
 * scenes, uncoment the print statments. That shows you the index, plaintext
 * for of the letter, followed by the plugboard return, followed by the 7 rotor
 * outputs (0, 1, 2, 3, 2, 1, 0) (3 = reflector), followed by the last
 * plugboard output.
 */

int encrypt_string(char **plaintext_ptr) {
  if ((!plaintext_ptr) || (!*plaintext_ptr) || (strlen(*plaintext_ptr) <= 0))
    return BAD_INPUT;

  char *plaintext = *plaintext_ptr;

  session_t *sesh = get_settings();

  int i = 0;
  while (plaintext[i] != 0) {
    //printf("%d<%c> ", i, plaintext[i]);
    char buff = encrypt_letter(sesh, plaintext[i]);

    if (buff == -1)
      break;
    if (buff != 0)
      plaintext[i] = buff;

    //printf(" %c\n", plaintext[i]);
    i++;
  }
  set_settings(sesh);
  close_session(&sesh, NULL);
  return i;
} /* encrypt_string() */

char encrypt_letter(session_t *sesh, char letter) {
  if ((!sesh) || (letter == 0))
    return -1;

  if ((letter <= 64) || ((letter > 90) && (letter < 97)) ||
      (letter > 122)) {
    //printf("\n");
    return 0;
  }
  //printf("%c ", letter);
  char buff = 0;
  letter = plugboard(sesh, &letter);

  for (int j = 0; j < 4; j++) {
    buff = rotor(sesh, j, &letter);
    if (buff != 0)
      letter = buff;
    //printf("%c", letter);
  }

  for (int j = 2; j >= 0; j--) {
    buff = rotor(sesh, j, &letter);
    if (buff != 0)
      letter = buff;
    //printf("%c", letter);
  }

  letter = plugboard(sesh, &letter);

  click(&sesh, 0);
  return letter;
} /* encrypt_char() */

