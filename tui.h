#ifndef TUI_H

#include <curses.h>

#define MENU_CURSOR   (ACS_RARROW)
#define ERROR         (-1)
#define NULL_INPUT    (-2)

int draw_blank_scr(WINDOW *header_w);
int draw_keyboard(WINDOW *keyboard_w, char key);
int draw_output(WINDOW *output_w, char *plain, char *cipher);
int menu(WINDOW *sidebar_w);

#endif   /* TUI_h */
