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
  //WINDOW *sidebar_w = subwin(stdscr, YMAX - 8, 29, 8, 0);
  WINDOW *active_w = subwin(stdscr, YMAX - 8, XMAX, 8, 0);
  //WINDOW *pause_w = subwin(stdscr, YMAX - 8, XMAX - 28, 8, 28);

  WINDOW *keyboard_w = subwin(active_w, 20, XMAX, YMAX - 20, 0);
  //WINDOW *output_w = subwin(active_w, 25, XMAX, 0, 0);

  draw_blank_scr(header_w);
  draw_keyboard(keyboard_w, 0);
  wrefresh(active_w);
  refresh();

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
      draw_keyboard(active_w, action);
      cipher[i] = encrypt_letter(&sesh, (char) action);
      plain[i] = action;
    }
    else if (action == 27) {
      break;
      //show menu
    }
    wrefresh(active_w);
    refresh();
    i++;
  }

  set_settings(sesh);
  close_session(&sesh, NULL);
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
      wattron(keyboard_w, A_STANDOUT);
      mvwaddch(keyboard_w, y_pos, x_pos, key_order[i]);
      wattroff(keyboard_w, A_STANDOUT);
    }

    else
      mvwaddch(keyboard_w, y_pos, x_pos, key_order[i]);
  }
  wrefresh(keyboard_w);
  return highlighted_key;
}

int menu(WINDOW *sidebar_w) {
  if (!sidebar_w)
    return -1;

  char **messages = {"1. go back:", "2. edit settings:", "3. exit:"};

  int selected = 1;
  while (TRUE) {
    box(sidebar_w, '-', '|');
    for (int i = 0; i < 3; i++) {
      if (selected % 3 == i) {
        wattron(sidebar_w, A_STANDOUT);
        mvwaddstr(sidebar_w, i + 8, 1, messages[i]);
        mvwaddch(sidebar_w, i + 8, 0, A_RARROW);
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
  if (selected == 0)
    return 0;
  if (selected == 3)
    return -1;

  // draw edit buttons
}
