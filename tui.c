#include "tui.h"
#include "init.h"
#include "encrypt.h"

#include <curses.h>
#include <stdio.h>
#include <strings.h>
#include <stdbool.h>

static int YMAX, XMAX = 0;

int main(int argc, char **argv) {
  initscr();            /* start da machine                       */
  noecho();             /* don't show what we input               */
  curs_set(0);          /* don't show the cursor                  */
  keypad(stdscr, TRUE); /* allow us to type in special characters */
  raw();                /* disable line buffering                 */

  getmaxyx(stdscr, YMAX, XMAX);

  WINDOW *header_w = subwin(stdscr, 9, XMAX,  0, 0);
  //WINDOW *setting_w = subwin(stdscr, YMAX - 8, 29, 8, 0);
  WINDOW *active_w = subwin(stdscr, YMAX - 8, XMAX, 8, 0);
  //WINDOW *inactive_w = subwin(stdscr, YMAX - 8, XMAX - 28, 8, 28);

  //WINDOW *keyboard_w = subwin(stdscr, 16, XMAX, YMAX - 16, 0);
  //WINDOW *output_w = subwin(stdscr, 25, XMAX, 8, 0);

  draw_blank_scr(header_w);
  draw_keyboard(active_w, 'a');

  session_t *sesh = get_settings();
  char plain[129] = { 0 };
  char cipher[129] = { 0 };
  int i = 0;

  while (TRUE) {
    int action = getch();
    if (i == 129) {
      break;
      // will put in a function to save plain and cipher and reset them later.
    }
    if ((action >= 97) && (action <= 122)) {
      draw_keyboard(active_w, action - 97);
      cipher[i] = encrypt_letter(sesh, (char) action);
      plain[i] = action;
    }
    else if (action == 27) {
      break;
      wclear(stdscr);
      //show menu
    }
    wrefresh(active_w);
    refresh();
    i++;
  }


  getch();
  endwin();

  printf("plaintext: %s\nciphertext: %s\n", plain, cipher);
  return 0;
}

int draw_blank_scr(WINDOW *header_w) {
  box(stdscr, '|', '-');
  hline('-', XMAX - 2);
  mvwaddstr(header_w, 3, XMAX / 2 - 9, "THE ENIGMA MACHINE");
  refresh();
  return 0;
}

int draw_keyboard(WINDOW *active_w, char key) {
  char *keyboard_alpha = "qwertyuiopasdfghjklzxcvbnm ";

  int a_xmax, a_ymax = 0;
  getmaxyx(active_w, a_ymax, a_xmax);

  int highlighted_key = -1;
  for (int i = 0; i < 26; i++)
    if (keyboard_alpha[i] == key)
      highlighted_key = i;

  int x_pos, y_pos = 0;
  for (int i = 0; i < 26; i++) {
    if (i <= 9) {
      y_pos = a_ymax - 15;
      x_pos = a_xmax / 2 + i * 9 - 41;
    }
    else if (i <= 18) {
      y_pos = a_ymax - 8;
      x_pos = a_xmax / 2 + (i - 9) * 9 - 46;
    }
    else {
      y_pos = a_ymax - 2;
      x_pos = a_xmax / 2 + (i - 18) * 9 - 41;
    }

    if (highlighted_key == i) {
      attron(A_REVERSE);
      mvwaddch(active_w, y_pos, x_pos, keyboard_alpha[i]);
      attroff(A_REVERSE);
    }

    else
      mvwaddch(active_w, y_pos, x_pos, keyboard_alpha[i]);
  }
  wrefresh(active_w);
  return 0;
}
