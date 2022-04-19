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

void print_settings(settings_struct_t *settings) {
  if (settings == 0) {
    return;
  }
  printf("Enigma settings:\n");

  printf("Rotors:\n");
  for (int i = 0; i < 3; i++) {
    printf("%d: %s %d\n", i, settings->rotors[i], settings->r_set[i]);
  }

  printf("Plugboard:\n");
  for (int i = 0; i < 10; i++) {
    printf("%c-%c ", settings->plug_top[i], settings->plug_bot[i]);
  }
  printf("\n");

}

settings_struct_t *get_settings() {
  FILE *in_ptr = NULL;
  in_ptr = fopen(".settings.txt", "r");
  if (in_ptr == NULL) {
    printf("Error, no file found.");
    return NULL;
  }

  settings_struct_t *settings = malloc(sizeof(settings_struct_t));
  if (settings == NULL) {
    return NULL;
  }

  int status = 0;
  for (int i = 0; i < 3; i++) {
    status = fscanf(in_ptr, "%d ", &(settings->r_pos[i]));
    if (status != 1) {
      return NULL;
    }
  }

  for (int i = 0; i < 3; i ++) {
    strncpy(settings->rotors[i], G_ROTORS[settings->r_pos[i] % 8], 27);
  }
  strncpy(settings->rotors[3], G_ROTORS[8], 27);

  for (int i = 0; i < 3; i++) {
    status = fscanf(in_ptr, "%d ", &(settings->r_set[i]));
    if(status != 1) {
      return NULL;
    }
  }

  for (int i = 0; i < 10; i++) {
    status = fscanf(in_ptr, "%c ", &(settings->plug_top[i]));
    if(status != 1) {
      return NULL;
    }
  }

  for (int i = 0; i < 10; i++) {
    status = fscanf(in_ptr, "%c ", &(settings->plug_bot[i]));
    if(status != 1) {
      return NULL;
    }
  }

  in_ptr = NULL;

  return settings;
}

int set_settings(settings_struct_t *settings) {
  FILE *out_ptr = NULL;
  out_ptr = fopen(".settings.txt", "w");
  if (out_ptr == NULL) {
    printf("Error, no file found.");
    return STARTUP_ERR;
  }

  if (settings == NULL) {
    return STARTUP_ERR;
  }

  int status = 0;
  for (int i = 0; i < 3; i++) {
    status = fprintf(out_ptr, "%d ", settings->r_pos[i]);
    if (status <= 0) {
      return WRITE_ERR;
    }
  }
  fprintf(out_ptr, "\n");

  for (int i = 0; i < 3; i++) {
    status = fprintf(out_ptr, "%d ", settings->r_set[i]);
    if(status <= 0) {
      return WRITE_ERR;
    }
  }
  fprintf(out_ptr, "\n");

  for (int i = 0; i < 10; i++) {
    status = fprintf(out_ptr, "%c ", settings->plug_top[i]);
    if(status <= 0) {
      return WRITE_ERR;
    }
  }
  fprintf(out_ptr, "\n");

  for (int i = 0; i < 10; i++) {
    status = fprintf(out_ptr, "%c ", settings->plug_bot[i]);
    if(status <= 0) {
      return WRITE_ERR;
    }
  }

  printf("close: %d\n", fclose(out_ptr));
  out_ptr = NULL;

  return 0;
}

