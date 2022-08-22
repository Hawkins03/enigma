#include "init.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
  clear_messages();
  char *message = malloc(129);

  if (!message)
    return -1;

  for (int i = 0; i < 129; i++)
    message[i] = 'a';
  message[128] = '\0';

  if (WRITE_ERR == -3)
    printf("test start:\n");

  int msg_num = 0;
  char *recv;
  for (int i = 0; i < 3; i++) {
    int status = append_message(&msg_num, message);
    printf("%d %s\n", msg_num, (status == 1) ? "Write Worked!" : "Write Failed");

    char *recv = read_message(msg_num);
    recv[128 * (i - 1)] = 'a';
    if (recv)
      printf("%s\n", recv);
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
