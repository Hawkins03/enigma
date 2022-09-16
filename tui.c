#include "tui.h"

static int XMAX = 0;
static int YMAX = 0;
static int MENU_MARGIN = 5;

/* TODO:
 *  - make more better and check to see why the screen flickers when you type
 *  fast enough
 *
 *  - add visualization of r_set + r_pos under the header
 *    (like the real version)
 *
 *  - make sure to just put this in a for loops (size 128) that auto-resets
 *  i and plain, etc.
 *
 *  X add documentation
 *
 *  - add help strings
 *
 *  - move display_output into 2 functions
 *
 *  - move the refrence to reset_screen out of menu
 *
 * ERROR DOCUMENTATION:
 * 1. if you stack overflow around when you get to the end of the buffer, make
 * sure i is being reset when you hit 128 characters. 
 *      -- FIXED
 *
 * 2. make sure to include exceptions if a user hits a non alphabetic character
 *      -- FIXED
 */

int main(int argc, char **argv) {
  int *plain_messages = malloc(sizeof(int));
  int *cipher_messages = malloc(sizeof(int));
  if ((!plain_messages) || (!cipher_messages))
    return STARTUP_ERR;

  initscr();            /* start da machine                       */
  noecho();             /* don't show what we input               */
  curs_set(0);          /* don't show the cursor                  */
  keypad(stdscr, TRUE); /* allow us to type in special characters */
  raw();                /* disable line buffering                 */

  getmaxyx(stdscr, YMAX, XMAX);

  /*
   * window breakdown:
   * header: top of screen
   * keyboard: where the "keyboard" shows up
   * output: where I output the results
   * menu: the whole screen - the margin
   */

  //-------------------WINDOWS-------------------------
  WINDOW *header_w = subwin(stdscr, 9, XMAX,  0, 0);
  WINDOW *keyboard_w = newwin(20, XMAX - 2, YMAX - 21, 1);
  WINDOW *output_w = newwin(3, XMAX - 4, 31, 2);
  WINDOW *menu_w = newwin(YMAX - 2 * MENU_MARGIN, XMAX - 2 * MENU_MARGIN,
                          MENU_MARGIN, MENU_MARGIN);

  //----    // if there is, then just do it.
  //---------------VARIABLES-----------------------
  session_t *sesh = get_settings();
  char plain[129] = { 0 };
  char cipher[129] = { 0 };
  int i = 0;

  //-------------------STARTUP-------------------------
  clear_messages(".plaintext.txt");
  clear_messages(".ciphertext.txt");

  draw_blank_scr(header_w);
  draw_keyboard(keyboard_w, 0);
  draw_output(output_w, plain, cipher);
  refresh();

  while (TRUE) {
    int action = getch();
    // ----------------CLEARING BUFFER----------------
    if (strlen(plain) == 128) {
      int status = append_message(plain_messages, plain, ".plaintext.txt");
      if (status != 1)
        break;
      status = append_message(cipher_messages, cipher, ".ciphertext.txt");
      if (status != 1)
        break;
      i = 0;
      memset(plain, 0, strlen(plain));
      memset(cipher, 0, strlen(cipher));
    }

    // ----------------ENCRYPTING---------------------
    if (islower(action)) {
      cipher[i] = encrypt_letter(&sesh, (char) action);
      plain[i] = action;
      draw_keyboard(keyboard_w, cipher[i]);
      draw_output(output_w, plain, cipher);
    }

    else if (isupper(action)) {
      cipher[i] = toupper(encrypt_letter(&sesh, (char) tolower(action)));
      plain[i] = action;
      draw_output(output_w, plain, cipher);
      draw_keyboard(keyboard_w, cipher[i] + 32);
    }
    // escape key -> menu
    else if (action == 27) {
      int status = menu(menu_w, header_w, keyboard_w, output_w, plain, cipher,
                        &sesh);
      if (status == -1)
        break;
    }
    // other keypresses
    else {
      if ((action >= 32) && (action <= 126)) {
        plain[i] = action;
        cipher[i] = action;
      }
      draw_keyboard(keyboard_w, 0);
      draw_output(output_w, plain, cipher);
    }


    //making the display work
    wrefresh(keyboard_w);
    wrefresh(output_w);
    refresh();
    i++;
  }

  // clearing variables
  delwin(header_w);
  delwin(keyboard_w);
  delwin(output_w);

  set_settings(sesh);

  // ending
  close_session(&sesh, NULL);
  endwin();

  if (*plain_messages > 0) {
    printf("Plaintext:\n");
    print_messages(*plain_messages, ".plaintext.txt");
    printf("%s\n", plain);
    printf("Ciphertext:\n");
    print_messages(*cipher_messages, ".ciphertext.txt");
    printf("%s\n", cipher);
  }
  else
    printf("Plaintext:  %s\nCiphertext: %s\n", plain, cipher);

  free(plain_messages);
  free(cipher_messages);
  plain_messages = NULL;
  cipher_messages = NULL;

  return 0;
}

/* displays the header, and draws a box around the screen and header. */
int draw_blank_scr(WINDOW *header_w) {
  box(stdscr, '|', '-');
  mvwaddstr(header_w, 3, XMAX / 2 - 9, "THE ENIGMA MACHINE");
  refresh();
  return 1;
}

/*
 * displays a keyboard, and highlights the selected key.
 * to not highlight a key, just set key to -1.
 *
 * no return because no error opportunities.
 */
int draw_keyboard(WINDOW *keyboard_w, char key) {
  if (!keyboard_w)
    return -1;
  // if you have a different layout, edit here
  char *key_order = "qwertyuiopasdfghjklzxcvbnm";

  // finding size of keyboard
  int key_xmax, key_ymax = 0;
  getmaxyx(keyboard_w, key_ymax, key_xmax);

  // finding right key to highlight
  int highlighted_key = -1;
  for (int i = 0; i < 26; i++)
    if (key_order[i] == key)
      highlighted_key = i;

  // finding right y + x positioning
  int x_pos, y_pos = 0;
  for (int i = 0; i < 26; i++) {
    if (i <= 9) {
      y_pos = key_ymax - 17;
      x_pos = key_xmax / 2 + i * 9 - 41;
    }
    else if (i <= 18) {
      y_pos = key_ymax - 10;
      x_pos = key_xmax / 2 + (i - 9) * 9 - 46;
    }
    else {
      y_pos = key_ymax - 3;
      x_pos = key_xmax / 2 + (i - 18) * 9 - 41;
    }

    // displaying keys
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
  return 1;
}

/* displays the plain and ciphertext.
 * currently this function is ineffecient. I don't want to re-display everything
 * each time I enter a key, I just want to have to display 1. (look into making
 * another function to just display 1, and move this over to reset_screen)
 */

int draw_output(WINDOW *output_w, char *plain, char *cipher) {
  wclear(output_w);
  if ((!plain) || (!cipher))
    return -1;

  wprintw(output_w, "Plaintext: %s\n", plain);
  wprintw(output_w, "\nCiphertext: %s\n", cipher);

  wrefresh(output_w);
  return 1;
}


/* Displays a menu allowing you to either return to normal use, edit setting,
 * or exit from the program.
 *
 * has three options that you can navigate using arrow keys:
 * 1. go back (go back to normal functioning)
 * 2. edit settings (if you have a specific setting you want to use)
 * 3. exit to the terminal (can just hit esc again as well.)
 */
int menu(WINDOW *menu_w, WINDOW *header_w, WINDOW *keyboard_w,
         WINDOW *output_w, char *plain, char *cipher, session_t **sesh_ptr) {
  // does everything I'm using exist?
  if ((!menu_w) || (!menu_w) || (!header_w) || (!keyboard_w) || (!output_w) ||
      (!plain) || (!cipher) || (!sesh_ptr) || (!(*sesh_ptr)))
    return NULL_INPUT;

  //clearing the screen
  wclear(menu_w);
  wrefresh(menu_w);
  box(menu_w, '|', '-');

  char *messages[3] = {"1. go back:", "2. edit settings:", "3. exit:"};
  int selected = 0;

  while (TRUE) {
    // displaying the menu
    for (int i = 0; i < 3; i++) {
      if ((3 + selected) % 3 == i) {
        wattron(menu_w, A_STANDOUT);
        mvwaddstr(menu_w, i + 8, XMAX / 2 - 10, messages[i]);
        mvwaddch(menu_w, i + 8, XMAX / 2 - 11, ACS_RARROW);
        wattroff(menu_w, A_STANDOUT);
      }
      else {
        mvwaddstr(menu_w, i + 8, XMAX / 2 - 10, messages[i]);
        mvwaddch(menu_w, i + 8, XMAX / 2 - 11, ACS_DIAMOND);
      }
    }
    wrefresh(menu_w);
    refresh();

    // getting an input
    int action = getch();
    if ((action == KEY_DOWN) || (action == '\t'))
      selected = (selected + 1) % 3;
    else if (action == KEY_UP)
      selected = (selected + 2) % 3;
    else if ((action == '\n') || (action == ' ')) {
      // selected an option
      if ((3 + selected) % 3 == 0) { // go back
        reset_screen(header_w, keyboard_w, output_w, plain, cipher);
        return 1;
      }
      else if ((3 + selected) % 3 == 1) { // edit settings
        draw_settings(menu_w, sesh_ptr);
        wclear(menu_w);
        wrefresh(menu_w);
        refresh();
      }
      else { // quit application
        return -1;
      }
    }
    else if (action == 27) // quit application pt 2.
      return -1;
  }

  selected = selected % 3;
  return selected;
}


/* clears everything on the screen and re-displays it after going back from
 * using the menu - i.e. the reason why menu is so bloated.
 */
int reset_screen(WINDOW *header_w, WINDOW *keyboard_w, WINDOW *output_w,
                 char *plain, char *cipher) {
  clear();
  draw_blank_scr(header_w);
  wrefresh(header_w);
  draw_keyboard(keyboard_w, 0);
  wrefresh(keyboard_w);
  wclear(output_w);
  draw_output(output_w, plain, cipher);
  wrefresh(output_w);
  refresh();

  return 1;
}

/* I really hate that math doesn't work how I want it to.
 * Like any normal person, I use modulo to loop around numbers when I need a
 * display to go from index 0 to index max. I spent about half an hour figuring
 * out that
 * a % b != (b + a) % b
 *
 * fyi:
 * ((-1) / 26) * 26 + (-1) % 26 = -1
 * (0) * 26 + (-1) % 26 = -1
 * 0 + (-1) % 26 = -1
 * (-1) % 26 = -1
 *
 * solution:
 * a % b -- NO
 * (b + a) % b -- YES
 *
 * anyways, this function displays the settings and then kicks out to menu when
 * needed. It's really cardboard and duct-tape, so I've got to improve it later.
 */
int draw_settings(WINDOW *menu_w, session_t **sesh_ptr) {
  //does everything exist?
  if ((!menu_w) || (!sesh_ptr) || (!(*sesh_ptr)))
    return NULL_INPUT;

  //clear the screen
  wclear(menu_w);
  box(menu_w, '|', '-');
  wrefresh(menu_w);
  refresh();

  session_t *sesh = *sesh_ptr;
  char *headers[4] = {"Rotors:", "Rotor settings:", "Plugboard a:",
                 "Plugboard b:"};

  int column = 0;
  int row = 0;
  int depth = 0;

  while (TRUE) {
    column = (4 + column) % 4;
    row = (10 + row) % 10;
    //displaying rows:
    for (int i = 0; i < 4; i++) {
      if ((4 + column) % 4 == i) {
        wattron(menu_w, A_STANDOUT);
        mvwaddstr(menu_w, i + 8, XMAX / 2 - 19, headers[i]);
        mvwaddch(menu_w, i + 8, XMAX / 2 - 20, ACS_RARROW);
        wattroff(menu_w, A_STANDOUT);
      }
      else {
        mvwaddstr(menu_w, i + 8, XMAX / 2 - 19, headers[i]);
        mvwaddch(menu_w, i + 8, XMAX / 2 - 20, ACS_DIAMOND);
      }
    }

    //displaying rotors
    for (int j = 0; j < 3; j++) {
      if (((10 + row) % 10 == j) && ((4 + column) % 4 == 0)) {
        wattron(menu_w, A_STANDOUT);
        mvwaddch(menu_w, 8, XMAX / 2 - 2 + 2 * j, sesh -> r_pos[j] + 48);
        wattroff(menu_w, A_STANDOUT);
      }
      else
        mvwaddch(menu_w, 8, XMAX / 2 - 2 + 2 * j, sesh -> r_pos[j] + 48);
    }

    //displaying rotor settings
    for (int j = 0; j < 3; j++) {
      if (((3 + row) % 3 == j) && ((4 + column) % 4 == 1)) {
        wattron(menu_w, A_STANDOUT);
        mvwaddch(menu_w, 9, XMAX / 2 - 2 + 2 * j,
                 sesh->rotors[sesh->r_pos[j]][(sesh->r_set[j]) % 26]);
        wattroff(menu_w, A_STANDOUT);
      }
      else if (j < 3)
        mvwaddch(menu_w, 9, XMAX / 2 - 2 + 2 * j,
                 sesh->rotors[sesh->r_pos[j]][(sesh->r_set[j]) % 26]);
    }

    //displaying plugboard a
    for (int j = 0; j < 10; j++) {
      if ((row % 10 == j) && (column % 4 == 2)) {
        wattron(menu_w, A_STANDOUT);
        mvwaddch(menu_w, 10, XMAX / 2 - 2 + 2 * j, sesh->plug_top[j]);
        wattroff(menu_w,A_STANDOUT);
      }
      else
        mvwaddch(menu_w, 10, XMAX / 2 - 2 + 2 * j, sesh->plug_top[j]);
    }

    //displaying plugboard b
    for (int j = 0; j < 10; j++) {
      if (((10 + row) % 10 == j) && ((4 + column) % 4 == 3)) {
        wattron(menu_w, A_STANDOUT);
        mvwaddch(menu_w, 11, XMAX / 2 - 2 + 2 * j, sesh->plug_bot[j]);
        wattroff(menu_w,A_STANDOUT);
      }
      else
        mvwaddch(menu_w, 11, XMAX / 2 - 2 + 2 * j, sesh->plug_bot[j]);
    }

    wrefresh(menu_w);
    refresh();

    int action = getch();

    //selecting row
    if ((3 + depth) % 3 == 0) {
        if (action == 27)
          return 0;
        else if ((action == KEY_DOWN) && (depth == 0))
          column++;
        else if ((action == KEY_UP) && (depth == 0))
          column--;
        else if (action == '\n')
          depth++;
    }

    //selecting thing to edit
    else if ((3 + depth) % 3 == 1) {
        if (action == KEY_RIGHT)
          row++;
        else if (action == KEY_LEFT)
          row--;
        else if (action == 27) {
          row = 0;
          depth--;
        }
        else if (action == '\n')
          depth++;
    }

    // editing thing.
    else if ((3 + depth) % 3 == 2) {
      if ((action == 27) || (action == '\n'))
        depth--;
      if (action == KEY_DOWN) { // ADD
        if ((4 + column) % 4 == 0) // rotor
          sesh->r_pos[(3 + row) % 3] = (8 + sesh->r_pos[(3 + row) % 3] + 1) % 8;
        else if ((4 + column % 4) == 1) // rotor setting
          sesh->r_set[(3 + row) % 3] = (26 + sesh->r_set[(3 + row) % 3] + 1) %
                                       26;
        else if (((4 + column) % 4) == 2) // plugboard a
          sesh->plug_top[(3 + row) % 3] = 26 + (sesh->plug_top[(3 + row) % 3]
                                           - 96) % 26 + 97;
        else if ((4 + column) % 4 == 3) // plugboard b
          sesh->plug_bot[(3 + row) % 3] = (sesh->plug_top[(3 + row) % 3] - 96)
                                          % 26 + 97;
      }
      else if (action == KEY_UP) { // SUBTRACT
        if ((4 + column) % 4 == 0) // rotor
          sesh->r_pos[(3 + row) % 3] = 8 + (sesh->r_pos[(3 + row) % 3] - 1) % 8;
        else if (column % 4 == 1) // rotor setting
          sesh->r_set[(3 + row) % 3] = 26 + (sesh->r_set[(3 + row) % 3] - 1)
                                       % 26;
        else if (column % 4 == 2) // plugboard a
          sesh->plug_top[(3 + row) % 3] = (26 + sesh->plug_top[(3 + row) % 3] 
                                          - 98) % 26 + 97;
        else // plugboard b
          sesh->plug_bot[(3 + row) % 3] = (26 + (sesh->plug_top[row % 3] - 98))
                                          % 26 + 97;
      }
    }
  }
  return 1;
}
