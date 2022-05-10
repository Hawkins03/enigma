#include "init.h"

#include <stdio.h>
#include <string.h>
#include <malloc.h>

// all 8 rotors that you can select plus the reflector.
const char G_ROTORS[9][27] = {"ekmflgdqvzntowyhxuspaibrcj",
                               "ajdksiruxblhwtmcqgznpyfvoe",
                               "bdfhjlcprtxvznyeiwgakmusqo",
                               "esovpzjayquirhxlnftgkdcmwb",
                               "vzbrgityupsdnhlxawmjqofeck",
                               "jpgvoumfyqbenhzrdkasxlitcw",
                               "nzjhgrcxmyswboufaivlpekqdt",
                               "fkqhtlxocbjspdzramewniuygv",
                               "ejmzalyxvbwfcrquontspikhgd"};

/*
 * prints a display of all the settings saved in the session.
 */

void print_settings(session_t *session) {
  if (!session)
    return;

  printf("Enigma settings:\n");

  printf("Rotors:\n");
  for (int i = 0; i < 3; i++) {
    printf("%d: %s %d\n", session->r_pos[i], session->rotors[i],
           session->r_set[i]);
  }

  printf("Plugboard:\n");
  for (int i = 0; i < 10; i++) {
    printf("%c-%c ", session->plug_top[i], session->plug_bot[i]);
  }

  printf("\n");
} /* print_settings() */

/*
 * Opens the session file, writes it to the heap, and outputs it. (Note, it
 * must be closed before ending the program (see close_session()))
 *
 * the session file is hidden because it will break the program if you mess it
 * up. For that reason, I would ask you edit the settings with a provided
 * function (TO BE CREATED).
 *
 * Will return null if the file can't be opened, if the session can't be
 * malloc'd, or if the data can't be read in. (See goto statments).
 *
 * I know it's sometimes looked down upon to use goto statments, however, I
 * believe that it's allowed in this context. Otherwise it would I would have
 * to make a more complicated function to close it anyways. This way it's
 * easier to read (and uses less memory).
 */

session_t *get_settings() {
  FILE *in_ptr = NULL;
  in_ptr = fopen(".session.txt", "r");
  if (in_ptr == NULL) {
    printf("Error, no file found.");
    return NULL;
  }

  // mallocing
  session_t *session = malloc(sizeof(session_t));
  if (!session) {
    fclose(in_ptr);
    in_ptr = NULL;
    return NULL;
  }

  // reading in the rotor numbers.
  int status = 0;
  for (int i = 0; i < 3; i++) {
    status = fscanf(in_ptr, "%d ", &(session->r_pos[i]));
    if (status != 1) {
      close_session(&session, &in_ptr);
      return NULL;
    }
  }

  // Selecting the rotors.
  for (int i = 0; i < 3; i ++) {
    strncpy(session->rotors[i], G_ROTORS[session->r_pos[i] % 8], 27);
  }
  // Selecting the reflector rotor.
  strncpy(session->rotors[3], G_ROTORS[8], 27);

  // reading in the rotor settings.
  for (int i = 0; i < 3; i++) {
    status = fscanf(in_ptr, "%d ", &(session->r_set[i]));
    if(status != 1) {
      close_session(&session, &in_ptr);
      return NULL;
    }
  }

  // getting the plugboard rows
  for (int i = 0; i < 10; i++) {
    status = fscanf(in_ptr, "%c ", &(session->plug_top[i]));
    if(status != 1) {
      close_session(&session, &in_ptr);
      return NULL;
    }
  }

  for (int i = 0; i < 10; i++) {
    status = fscanf(in_ptr, "%c ", &(session->plug_bot[i]));
    if(status != 1) {
      close_session(&session, &in_ptr);
      return NULL;
    }
  }

  // program success.
  fclose(in_ptr);
  in_ptr = NULL;
  return session;
} /* get_settings() */

/*
 * saves the settings to the session file.
 *
 * Returns startup error if there's an error opening the session file.
 *
 * Returns write error if there's an error writing the settings to the file.
 */

int set_settings(session_t *session) {
  FILE *out_ptr = NULL;
  out_ptr = fopen(".session.txt", "w");
  if (out_ptr == NULL) {
    printf("Error, no file found.");
    return STARTUP_ERR;
  }

  if (session == NULL) {
    return STARTUP_ERR;
  }

  // writing the selected rotors
  int status = 0;
  for (int i = 0; i < 3; i++) {
    status = fprintf(out_ptr, "%d ", session->r_pos[i]);
    if (status <= 0) {
      fclose(out_ptr);
      out_ptr = NULL;
      return WRITE_ERR;
    }
  }
  fprintf(out_ptr, "\n");

  // writing the rotor settings
  for (int i = 0; i < 3; i++) {
    status = fprintf(out_ptr, "%d ", session->r_set[i]);
    if(status <= 0) {
      fclose(out_ptr);
      out_ptr = NULL;
      return WRITE_ERR;
    }
  }
  fprintf(out_ptr, "\n");

  // writing the plugboards
  for (int i = 0; i < 10; i++) {
    status = fprintf(out_ptr, "%c ", session->plug_top[i]);
    if(status <= 0) {
      fclose(out_ptr);
      out_ptr = NULL;
      return WRITE_ERR;
    }
  }
  fprintf(out_ptr, "\n");

  for (int i = 0; i < 10; i++) {
    status = fprintf(out_ptr, "%c ", session->plug_bot[i]);
    if(status <= 0) {
      fclose(out_ptr);
      out_ptr = NULL;
      return WRITE_ERR;
    }
  }

  fclose(out_ptr);
  out_ptr = NULL;

  return 1;
} /* set_settings() */

/*
 * closes the session, and optionally a file pointer as well. (Note, if you
 * set the file pointer to null, the program ignores it).
 */
int close_session(session_t **sesh_ptr, FILE **file_ptr) {
  if ((!sesh_ptr) || (!*sesh_ptr))
    return NULL_INPUT;

  if ((file_ptr) && (*file_ptr)) {
    fclose(*file_ptr);
    *file_ptr = NULL;
  }

  free(*sesh_ptr);
  *sesh_ptr = NULL;
  return 1;
} /* close_session() */

int edit_settings(session_t **sesh_ptr, int r_pos[3], int r_set[3],
                  unsigned char plug_top[10], unsigned char plug_bot[10]) {
  if ((!sesh_ptr) || (!*sesh_ptr))
    return 0;

  if (r_pos) {
    for (int i = 0; i < 3; i++)
      (*sesh_ptr)->r_pos[i] = r_pos[i];
    for (int i = 0; i < 3; i ++)
      strncpy((*sesh_ptr)->rotors[i], G_ROTORS[(*sesh_ptr)->r_pos[i] % 8], 27);
  }

  if (r_set)
    for (int i = 0; i < 3; i++)
      (*sesh_ptr)->r_set[i] = r_set[i];

  if (plug_top)
    for (int i = 0; i < 10; i++)
      (*sesh_ptr)->plug_top[i] = plug_top[i];

  if (plug_bot)
    for (int i = 0; i < 10; i++)
      (*sesh_ptr)->plug_bot[i] = plug_bot[i];



  return 1;
}
