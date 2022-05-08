#include "init.h"

#include <stdio.h>
#include <string.h>
#include <malloc.h>

const char G_ROTORS[9][27] = {"ekmflgdqvzntowyhxuspaibrcj",
                               "ajdksiruxblhwtmcqgznpyfvoe",
                               "bdfhjlcprtxvznyeiwgakmusqo",
                               "esovpzjayquirhxlnftgkdcmwb",
                               "vzbrgityupsdnhlxawmjqofeck",
                               "jpgvoumfyqbenhzrdkasxlitcw",
                               "nzjhgrcxmyswboufaivlpekqdt",
                               "fkqhtlxocbjspdzramewniuygv",
                               "ejmzalyxvbwfcrquontspikhgd"};

void print_settings(session_t *session) {
  if (session == 0) {
    return;
  }
  printf("Enigma settings:\n");

  printf("Rotors:\n");
  for (int i = 0; i < 3; i++) {
    printf("%d: %s %d\n", i, session->rotors[i], session->r_set[i]);
  }

  printf("Plugboard:\n");
  for (int i = 0; i < 10; i++) {
    printf("%c-%c ", session->plug_top[i], session->plug_bot[i]);
  }
  printf("\n");

}

session_t *get_settings() {
  FILE *in_ptr = NULL;
  in_ptr = fopen(".session.txt", "r");
  if (in_ptr == NULL) {
    printf("Error, no file found.");
    return NULL;
  }

  session_t *session = malloc(sizeof(session_t));
  if (!session)
    return NULL;

  int status = 0;
  for (int i = 0; i < 3; i++) {
    status = fscanf(in_ptr, "%d ", &(session->r_pos[i]));
    if (status != 1)
      return NULL;
  }

  for (int i = 0; i < 3; i ++) {
    strncpy(session->rotors[i], G_ROTORS[session->r_pos[i] % 8], 27);
  }
  strncpy(session->rotors[3], G_ROTORS[8], 27);

  for (int i = 0; i < 3; i++) {
    status = fscanf(in_ptr, "%d ", &(session->r_set[i]));
    if(status != 1)
      return NULL;
  }

  for (int i = 0; i < 10; i++) {
    status = fscanf(in_ptr, "%c ", &(session->plug_top[i]));
    if(status != 1)
      return NULL;
  }

  for (int i = 0; i < 10; i++) {
    status = fscanf(in_ptr, "%c ", &(session->plug_bot[i]));
    if(status != 1)
      return NULL;
  }

  in_ptr = NULL;

  return session;
}

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

  int status = 0;
  for (int i = 0; i < 3; i++) {
    status = fprintf(out_ptr, "%d ", session->r_pos[i]);
    if (status <= 0) {
      return WRITE_ERR;
    }
  }
  fprintf(out_ptr, "\n");

  for (int i = 0; i < 3; i++) {
    status = fprintf(out_ptr, "%d ", session->r_set[i]);
    if(status <= 0) {
      return WRITE_ERR;
    }
  }
  fprintf(out_ptr, "\n");

  for (int i = 0; i < 10; i++) {
    status = fprintf(out_ptr, "%c ", session->plug_top[i]);
    if(status <= 0) {
      return WRITE_ERR;
    }
  }
  fprintf(out_ptr, "\n");

  for (int i = 0; i < 10; i++) {
    status = fprintf(out_ptr, "%c ", session->plug_bot[i]);
    if(status <= 0) {
      return WRITE_ERR;
    }
  }

  printf("close: %d\n", fclose(out_ptr));
  out_ptr = NULL;

  return 1;
}

