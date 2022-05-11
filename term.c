#include "term.h"
#include "encrypt.h"
#include "init.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

long int find_str_len(char **str_lib, int len, char *padding, int start) {
  if ((!str_lib) || (!*str_lib))
    return NULL_INPUT;

  int sum = 1; // 1 bc NUL at the end
  for (int i = start - 1; i < len; i++) {
    sum += strlen(str_lib[i]);

    if (padding)
      sum += strlen(padding);
  }

  return sum;
}

char *join_str(char **str_lib, int len, char *padding, int start) {
  if (!(str_lib) || (!*str_lib))
    return NULL;

  if (find_str_len(str_lib, len, padding, start) == -1)
    return NULL;

  char *concat = malloc(find_str_len(str_lib, len, padding, start));
  if (concat == NULL)
    return NULL;

  for (int i = start - 1; i < len; i++) {
    strcat(concat, str_lib[i]);
    strcat(concat, padding);
  }

  return concat;
}

int main (int argc, char **argv) {

  char *buff = " ";
  char *pad = malloc(strlen(buff) + 1);
  if (pad == NULL) {
    printf("Error, malloc failed.");
    return -1;
  }

  strncpy(pad, buff, 2);

  char *concat = join_str(argv, argc, pad, 2);

  session_t *sesh = get_settings();
  if (sesh == NULL)
    return 0;

  printf("settings:\n");
  print_settings(sesh);
  close_session(&sesh, NULL);

  int status = encrypt_string(&concat);

  if (status < 1) {
    printf("ERROR, encryption failed. (error code %d)", status);
    return -1;
  }

  printf("Output: %s", concat);
  return 1;
}
