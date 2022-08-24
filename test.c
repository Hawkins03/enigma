#include "init.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
  clear_messages(".message.txt");
  char *message = malloc(129);

  if (!message)
    return -1;

  for (int i = 0; i < 128; i++)
    message[i] = 'a';
  message[128] = '\0';

  printf("Test Start:\n");

  int msg_num = 0;
  for (int i = 0; i < 3; i++) {
    // writing to file.
    int status = append_message(&msg_num, message, ".message.txt");
    printf("%d %s\n", msg_num,
           (status == 1) ? "Write Worked!" : "Write Failed");

    char *recv = read_message(msg_num, ".message.txt");

    if (recv)
      printf("Read Worked (%ld): %s\n", strlen(recv), recv);

    else {
      printf("Read Failed!\n");
      free(recv);
      recv=NULL;
      break;
    }
    free(recv);
    recv = NULL;
  }

  free(message);
  message = NULL;
  return 0;
}
