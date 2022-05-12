#ifndef TUI_H

#include <curses.h>
int draw_blank_scr(WINDOW *header_w);
int draw_keyboard(WINDOW *active_w, char key);


#endif   /* TUI_h */
