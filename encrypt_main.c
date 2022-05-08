#include "init.h"
#include "encrypt.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv) {
  session_t *sesh = get_settings();
  print_settings(sesh);

  char in = 'c';

  printf("encrypting 'c' was %s\n", (rotor(sesh, 1, &in) == 1)? "good": "bad");

  printf("letter is now %c\n\n", in);

  fflush(NULL);
  char *str = "stuff n things";
  char *m_str = malloc(strlen(str) + 1);

  printf("past malloc\n");
  fflush(NULL);

  if (!m_str) {
    printf("MALLOC ERROR\n");
    fflush(NULL);
    return 0;
  }
  printf("malloc good\n");
  fflush(NULL);

  strncpy(m_str, str, strlen(str) + 1);

  printf("str = %s\n", m_str);
  fflush(NULL);

  int status = encrypt_string(str);

  printf("new string is %s (status = %d)\n", str, status);
  fflush(NULL);

  status = set_settings(sesh);
  printf("%s", (status == 1) ? "session saved successfully" : "save failed");
  printf (" (%d)\n", status);

  free(m_str);
  m_str = NULL;

}
