#ifndef TUI_H

#include <curses.h>

#define MENU_CURSOR  (ACS_RARROW)
#define ERROR        (-1)

int draw_blank_scr(WINDOW *header_w);
int draw_keyboard(WINDOW *keyboard_w, char key);
int draw_output(WINDOW *output_w, char *plain, char *cipher);


#endif   /* TUI_h */
