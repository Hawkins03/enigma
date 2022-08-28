#include "encrypt.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/*
 * takes in a session, the rotor num (0-4) and a letter. (Note, this doesn't
 * check to see if the letter is in bounds, use encrypt_letter or
 * encrypt_string for that.)
 *
 * returns 0 of the letter is out of bounds, otherwise returns the encrypted
 * letter.
 */

char rotor(const session_t *sesh, int rotor_num, char letter) {
  if (!sesh)
    return 0;

  int rotor_pos = -1;
  if (letter < 97)
    rotor_pos = letter - 65;
  else
    rotor_pos = letter - 97;

  return sesh->rotors[rotor_num % 4][(rotor_pos + sesh->r_set[rotor_num]) % 26];
} /* rotor() */

/*
 * takes a character and if it matches a character on the plugboard, the
 * function returns the complimentary character back. (Not for personal use.)
 */

char plugboard(const session_t *sesh, char letter) {
  if (!sesh)
    return 0;

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
 * takes in a pointer to a session, and a rotor number. Then rotates the rotors
 * as in an actual enigma machine does once per key press (use
 * encrypt_letter() if you want to encrypt something, I don't recommend using
 * this function, but it is here if you need to use it.)
 */

int click(session_t **sesh_ptr, int rotor_num) {
  if ((!sesh_ptr) || (!(*sesh_ptr)))
    return BAD_INPUT;

  // kills the recursive loop (DO NOT TOUCH)
  if ((rotor_num >= 4) || (rotor_num < 0))
    return 0;

  int rotor_selection = (*sesh_ptr)->r_pos[rotor_num];
  const int turnover[8][2] = {{23, -1}, {22, -1}, {17, -1}, {20, -1}, {25, -1},
                              {12, 18}, {0, 16}, {16, 20}};

  (*sesh_ptr)->r_set[rotor_num] = ((*sesh_ptr)->r_set[rotor_num] + 1) % 26;

  if (((*sesh_ptr)->r_set[rotor_num] == turnover[rotor_selection][0]) ||
      ((*sesh_ptr)->r_set[rotor_num] == turnover[rotor_selection][1]))
    click(sesh_ptr, ++rotor_num);

  return 1;
} /* click() */

/*
 * !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 * IF THINGS GO WRONG REMOVE COMMENTING ON PRINTF STATMETS HERE!!!!!!!!!!!!!!!!!!!!!!!
 * !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 *
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
    char buff = encrypt_letter(&sesh, plaintext[i]);

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


/*
 * encrypts a single char (does in fact update sesh after.)
 *
 * IF YOU HAVE PROBLEMS UNCOMMENT PRINTF STATMENTS.
 *
 */
char encrypt_letter(session_t **sesh_ptr, char letter) {
  if ((!sesh_ptr) || (!*sesh_ptr) || (letter == 0))
    return -1;

  if ((letter <= 64) || ((letter > 90) && (letter < 97)) ||
      (letter > 122)) {
    //printf("\n");
    return 0;
  }

  //printf("%c ", letter);
  session_t *sesh = *sesh_ptr;
  char buff = 0;
  letter = plugboard(sesh, letter);

  for (int j = 0; j < 4; j++) {
    buff = rotor(sesh, j, letter);
    if (buff != 0)
      letter = buff;
    //printf("%c", letter);
  }

  for (int j = 2; j >= 0; j--) {
    buff = rotor(sesh, j, letter);
    if (buff != 0)
      letter = buff;
    //printf("%c", letter);
  }

  letter = plugboard(sesh, letter);
  //printf("%c\n", letter);

  click(&sesh, 0);
  return letter;
} /* encrypt_char() */
