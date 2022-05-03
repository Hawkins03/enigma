#include "init.h"

#include <stdio.h>
#include <stdlib.h>

int main() {
  session_t *session = get_settings();
  print_settings(session);
  printf("%d\n", set_settings(session));
  session = get_settings();
  print_settings(session);

  free(session);
  session = NULL;
}
