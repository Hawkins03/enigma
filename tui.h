#include "encrypt.h"
#include "init.h"

#include <ctype.h>
#include <ncurses.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef TUI_H

#define MENU_CURSOR   (ACS_RARROW)

int draw_blank_scr(WINDOW *header_w);
int draw_keyboard(WINDOW *keyboard_w, char key);
int draw_output(WINDOW *output_w, char *plain, char *cipher);
int menu(WINDOW *menu_w, WINDOW *header_w, WINDOW *keyboard_w, WINDOW *output_w,
         char *plain, char *cipher, session_t **sesh_ptr);
int reset_screen(WINDOW *header_w, WINDOW *keyboard_w,
                 WINDOW *output_w, char *plain, char *cipher);
int draw_settings(WINDOW *menu_w, session_t **sesh_ptr);

#endif   /* TUI_h */
