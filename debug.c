#include "init.h"
#include "encrypt.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv) {
  printf("Starting init debug:\n");
  printf("Getting settings:\n");
  session_t *sesh = get_settings();
  print_settings(sesh);

  printf("\nsaving settings:\n");
  int status = set_settings(sesh);
  printf("%s", (status == 1) ? "session saved successfully" : "save failed");
  printf(" (%d)\n", status);
  close_session(&sesh, NULL);

  printf("\nre-fetching settings:\n");
  sesh = get_settings();
  print_settings(sesh);

  close_session(&sesh, NULL);
  printf("\ninit debug done.\n\n");

  printf("encrypt debug:\n");

  char *str = "stuff n things";
  char *m_str = malloc(strlen(str) + 1);

  if (!m_str)
    return 0;

  strncpy(m_str, str, strlen(str) + 1);

  printf("str = %s\n", m_str);

  status = encrypt_string(&m_str);

  printf("new string is %s (status = %d)\n", m_str, status);

  free(m_str);
  m_str = NULL;

}
