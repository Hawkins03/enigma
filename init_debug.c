#include "init.h"

#include <stdio.h>
#include <stdlib.h>

int main() {
  settings_struct_t *settings = malloc(sizeof(settings_struct_t));
  settings = get_settings();
  print_settings(settings);
  printf("%d\n", set_settings(settings));
  settings = get_settings();
  print_settings(settings);

  free(settings);
  settings = NULL;
}
