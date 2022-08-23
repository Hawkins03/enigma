#include "tui.h"
#include "init.h"
#include "encrypt.h"

#include <ncurses.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

static int YMAX, XMAX = 0;

int main(int argc, char **argv) {
  initscr();            /* start da machine                       */
  noecho();             /* don't show what we input               */
  curs_set(0);          /* don't show the cursor                  */
  keypad(stdscr, TRUE); /* allow us to type in special characters */
  raw();                /* disable line buffering                 */

  getmaxyx(stdscr, YMAX, XMAX);

  /*
   * window breakdown:
   * header: top of screen
   * setting: left side, below header, shows settings
   * active: entire space below header
   * inactive: replaces active besides settings
   * keyboard: where the "keyboard" shows up
   * output: where I output the results
   */


  WINDOW *header_w = subwin(stdscr, 9, XMAX,  0, 0);
  WINDOW *keyboard_w = newwin(20, XMAX - 2, YMAX - 21, 1);
  WINDOW *output_w = newwin(3, XMAX - 2, 31, 1);

  draw_blank_scr(header_w);
  draw_keyboard(keyboard_w, 0);
  refresh();

  session_t *sesh = get_settings();
  char plain[129] = { 0 };
  char cipher[129] = { 0 };


  int i = 0;
  int plain_messages = 0;
  int cipher_messages = 0;
  while (TRUE) {
    wclear(output_w);
    int action = getch();
    if (i == 128) {
      int status = append_message(&plain_messages, plain, ".plaintext.txt");
      if (status != 1)
        break;
      status = append_message(&cipher_messages, cipher, ".ciphertext.txt");
      if (status != 1)
        break;
      memset(plain, 0, strlen(plain));
      memset(cipher, 0, strlen(cipher));
    }
    if ((action >= 97) && (action <= 122)) {
      draw_keyboard(keyboard_w, action);
      cipher[i] = encrypt_letter(&sesh, (char) action);
      plain[i] = action;
      draw_output(output_w, plain, cipher);
    }
    else if (action == 27) {
      break;
      //show menu
    }
    wrefresh(keyboard_w);
    wrefresh(output_w);
    refresh();
    i++;
  }

  delwin(header_w);
  delwin(keyboard_w);
  delwin(output_w);

  char *plain_out = read_message(plain_messages, ".plaintext.txt");
  char *cipher_out = read_message(cipher_messages, ".ciphertext.txt");
  if ((plain_out) && (cipher_out))
    printf("Plaintext: %s\nCiphertext: %s", plain_out, cipher_out);

  set_settings(sesh);

  free(plain_out);
  plain_out = 0;
  free(cipher_out);
  cipher_out = 0;

  clear_messages(".plaintext.txt");
  clear_messages(".ciphertext.txt");
  close_session(&sesh, NULL);
  endwin();

  return 0;
}

int draw_blank_scr(WINDOW *header_w) {
  box(stdscr, '|', '-');
  mvwaddstr(header_w, 3, XMAX / 2 - 9, "THE ENIGMA MACHINE");
  refresh();
  return 0;
}

int draw_keyboard(WINDOW *keyboard_w, char key) {
  char *key_order = "qwertyuiopasdfghjklzxcvbnm";

  int a_xmax, a_ymax = 0;
  getmaxyx(keyboard_w, a_ymax, a_xmax);

  int highlighted_key = -1;
  for (int i = 0; i < 26; i++)
    if (key_order[i] == key)
      highlighted_key = i;

  int x_pos, y_pos = 0;
  for (int i = 0; i < 26; i++) {
    if (i <= 9) {
      y_pos = a_ymax - 17;
      x_pos = a_xmax / 2 + i * 9 - 41;
    }
    else if (i <= 18) {
      y_pos = a_ymax - 10;
      x_pos = a_xmax / 2 + (i - 9) * 9 - 46;
    }
    else {
      y_pos = a_ymax - 3;
      x_pos = a_xmax / 2 + (i - 18) * 9 - 41;
    }

    if (highlighted_key == i) {
      wattron(keyboard_w, A_STANDOUT);
      mvwaddch(keyboard_w, y_pos, x_pos, key_order[i]);
      wattroff(keyboard_w, A_STANDOUT);
    }
    else {
      mvwaddch(keyboard_w, y_pos, x_pos, key_order[i]);
    }
  }
  wrefresh(keyboard_w);
  return highlighted_key;
}

int draw_output(WINDOW *output_w, char *plain, char *cipher) {
  if ((!plain) || (!cipher))
    return ERROR;

  wprintw(output_w, "Plaintext: %s\n", plain);
  wprintw(output_w, "\nCiphertext: %s\n", cipher);

  wrefresh(output_w);
  return 0;
}

int menu(WINDOW *sidebar_w) {
  if (!sidebar_w)
    return -1;

  char *messages[3] = {"1. go back:", "2. edit settings:", "3. exit:"};

  int selected = 1;
  while (TRUE) {
    box(sidebar_w, '-', '|');
    for (int i = 0; i < 3; i++) {
      if (selected % 3 == i) {
        wattron(sidebar_w, A_STANDOUT);
        mvwaddstr(sidebar_w, i + 8, 1, messages[i]);
        mvwaddch(sidebar_w, i + 8, 0, ACS_RARROW);
        wattroff(sidebar_w, A_STANDOUT);
        continue;
      }
      mvwaddstr(sidebar_w, i + 8, 1, messages[i]);
    }
    int action = getch();

    if (action == KEY_UP)
      selected++;
    else if (action == KEY_DOWN)
      selected--;
    else if (action == '\n')
      break;
    else if (action == 27) // escape or alt
      return -1;
  }

  selected = selected % 3;

  return selected;
}
