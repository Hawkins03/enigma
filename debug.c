#include "init.h"
#include "encrypt.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv) {
  printf("Starting settings access check:\n");

  printf("Fetching settings:\n");
  session_t *sesh = get_settings();
  print_settings(sesh);

  printf("saving settings:\n");
  int status = set_settings(sesh);

  printf("%s", (status == 1) ? "Session saved successfully" : "Save failed");
  printf(" (%d)\n\n", status);

  close_session(&sesh, NULL);

  printf("re-fetching settings:\n");
  sesh = get_settings();
  print_settings(sesh);

  close_session(&sesh, NULL);
  printf("\ninit debug done.\n\n");

  //-----------------------------------------------------
  printf("Starting message storage check\n");

  //mallocing message
  char message[129] = { 0 };
  memset(message, 'a', 128);
  message[128] = '\0';

  printf("Storing message: ");

  int msg_num = 0;
  status = append_message(&msg_num, message, ".message.txt");
  printf("%s\n", (status == 1) ? "success!\n":"fail :(\n");

  char *recv = read_message(msg_num, ".message.txt");
  if (recv)
    printf("read worked! %ld characters captured: %s\n", strlen(recv), recv);
  else
    printf("Read Failed.\n");
  free(recv);
  recv = NULL;

  printf("Printing message:\n");
  print_messages(1, ".message.txt");

  printf("Storing 2nd message: ");
  memset(message, 'b', 128);
  message[128] = '\0';
  status = append_message(&msg_num, message, ".message.txt");
  printf("%s\n", (status == 1) ? "worked!" : "failed");

  printf("reading in 2nd message: \n");
  recv = read_message(msg_num, ".message.txt"); //painfull line
  if (recv)
    printf("read worked! %ld characters captured: %s\n", strlen(recv), recv);
  else
    printf("read failed\n");
  free(recv);
  recv = NULL;

  printf("Printing 2 messages:\n");
  print_messages(2, ".message.txt");

  status = clear_messages(".message.txt");
  printf("clear messages: %s\n", (status == 1) ? "good!": "Bad :(");

  //------------------------------------------------------------
  printf("encrypt debug:\n");

  char *str = "stuff n things";
  char *m_str = malloc(strlen(str) + 1);

  if (!m_str)
    return -1;

  strncpy(m_str, str, strlen(str) + 1);

  printf("str = %s\n", m_str);

  status = encrypt_string(&m_str);

  printf("new string is %s (status = %d)\n", m_str, status);

  free(m_str);
  m_str = NULL;

}
