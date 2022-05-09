#include "init.h"

#include <stdio.h>
#include <stdlib.h>

int main() {
  session_t *session = get_settings();
  print_settings(session);
  session->r_set[0]++;
  printf("save was %s\n\n", set_settings(session)? "good": "bad");
  session = get_settings();
  print_settings(session);

  close_session(&session, NULL);
}

