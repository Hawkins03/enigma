#include "tui.h"


static int YMAX, XMAX = 0;



/*
 * Weird error documentation:
 * MAKE SURE THAT YOU RESET I IF YOU HIT THE END OF THE BUFFER, THIS CAUSES AN
 * OVERFLOW. (look into a nested for loop in the while loop)
 *
 * Capture dies if you try and enter a non-alphabetic character.
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
   * setting: left side, below header, shows settings
   * active: entire space below header
   * inactive: replaces active besides settings
   * keyboard: where the "keyboard" shows up
   * output: where I output the results
   */

  //-------------------WINDOWS-------------------------
  WINDOW *header_w = subwin(stdscr, 9, XMAX,  0, 0);
  WINDOW *keyboard_w = newwin(20, XMAX - 2, YMAX - 21, 1);
  WINDOW *output_w = newwin(3, XMAX - 2, 31, 1);
  WINDOW *menu_w = newwin(YMAX - 10, XMAX - 10, 5, 5);

  //-------------------VARIABLES-----------------------
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


  /* two different variables because they're not supposed to be messed with by
     the user */

  //MAIN_LOOP
  while (TRUE) {
    //wclear(output_w);
    int action = getch();
    //storing buffers into a file
    if (strlen(plain) == 128) {
      int status = append_message(plain_messages, plain, ".plaintext.txt");
      if (status != 1)
        break;
      status = append_message(cipher_messages, cipher, ".ciphertext.txt");
      if (status != 1)
        break;
      i = 0; // VERY IMPORTANT
      //clear buffers
      memset(plain, 0, strlen(plain));
      memset(cipher, 0, strlen(cipher));
    }

    //reading in letters
    if (islower(action)) {
      draw_keyboard(keyboard_w, action);
      cipher[i] = encrypt_letter(&sesh, (char) action);
      plain[i] = action;
      draw_output(output_w, plain, cipher);
    }
    else if (isupper(action)) {
      draw_keyboard(keyboard_w, action);
      cipher[i] = toupper(encrypt_letter(&sesh, (char) tolower(action)));
      plain[i] = action;
      draw_output(output_w, plain, cipher);
    }
    else if (action == 27) {
      int status = menu(menu_w);
      if (status == 0) {
        clear();
        draw_blank_scr(header_w);
        wrefresh(header_w);
        draw_keyboard(keyboard_w, 0);
        wrefresh(keyboard_w);
        wclear(output_w);
        draw_output(output_w, plain, cipher);
        wrefresh(output_w);
        refresh();
        continue;
      }
      else if ((status == -1) || (status == 2))
        break;
      else if (status == 3)
        break; // to be replaced with a settings menu later
      break; //only way to escape unless clearing the buffers fails.
      //show menu
    }
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

/* displays a keyboard, and highlights the selected key.
 *
 * returns the position of 
 */
int draw_keyboard(WINDOW *keyboard_w, char key) {
  // if you have a different layout, edit here
  char *key_order = "qwertyuiopasdfghjklzxcvbnm";

  // finding size of keyboard
  int a_xmax, a_ymax = 0;
  getmaxyx(keyboard_w, a_ymax, a_xmax);

  // finding right key to highlight
  int highlighted_key = -1;
  for (int i = 0; i < 26; i++)
    if (key_order[i] == key)
      highlighted_key = i;

  // finding right y + x positioning
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
  return highlighted_key;
}

/* displays the plain and ciphertext.
 * note, I'm investigating some weird error with this not displaying the
 * plaintext.
 *
 *
 * CURRENTLY KNOWN ERRORS:
 * if a non-alphabet character is typed, the function doesn't write anything
 * to the screen  - FIXED
 *
 * currently this is way too innefecient. After I finish drawing settings, I'm
 * going to make it just draw the singular character that was used.
 */

int draw_output(WINDOW *output_w, char *plain, char *cipher) {
  if ((!plain) || (!cipher))
    return ERROR;

  wprintw(output_w, "Plaintext: %s\n", plain);
  wprintw(output_w, "\nCiphertext: %s\n", cipher);

  wrefresh(output_w);
  return 1;
}


/* Displays a menu allowing you to either return to normal use, edit setting,
 * or exit from the program.
 *
 * NOTE, DOES NOT CURRRENTLY WORK.
 */
int menu(WINDOW *menu_w) {
  if (!menu_w)
    return NULL_INPUT;

  wclear(menu_w);
  wrefresh(menu_w);
  box(menu_w, '|', '-');

  char *messages[3] = {"1. go back:", "2. edit settings:", "3. exit:"};

  int selected = 0;

  while (TRUE) {
    for (int i = 0; i < 3; i++) {
      if (selected % 3 == i) {
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
    int action = getch();

    if ((action == KEY_DOWN) || (action == '\t'))
      selected++;
    else if (action == KEY_UP)
      selected--;
    else if ((action == '\n') || (action == ' '))
      break;
    else if (action == 27) // escape or alt
      return -1;
  }

  selected = selected % 3;
  return selected;
}

int draw_settings(WINDOW *menu_w, session_t **sesh_ptr, int column, int row) {
  if ((!menu_w) || (!sesh_ptr) || (!(*sesh_ptr)))
    return NULL_INPUT;

  session_t *sesh = *sesh_ptr;
  char *headers[4] = {"Rotors: ", "Rotor settings: ", "Plugboard a: ",
                      "Plugboard b: "};

  for (int i = 0; i < 4; i++) {
    if (column % 4 == i) {
      wattron(menu_w, A_STANDOUT);
      mvwaddstr(menu_w, i + 8, XMAX / 2 - 10, headers[i]);
      mvwaddch(menu_w, i + 8, XMAX / 2 - 11, ACS_RARROW);
      wattroff(menu_w, A_STANDOUT);
    }
    else {
      mvwaddstr(menu_w, i + 8, XMAX / 2 - 10, headers[i]);
      mvwaddch(menu_w, i + 8, XMAX / 2 - 11, ACS_DIAMOND);
    }
  }

  for (int j = 0; j < 3; j++) {
    if ((row % 3 == j) && (column % 4 == 0)) {
      wattron(menu_w, A_STANDOUT);
      mvwaddch(menu_w, 8, XMAX / 2 - 2 + 2 * j, sesh -> r_pos[j] + 48);
      wattroff(menu_w, A_STANDOUT);
    }
    else
      mvwaddch(menu_w, 8, XMAX / 2 - 2 + 2 * j, sesh -> r_pos[j] + 48);
  }

  for (int j = 0; j < 3; j++) {
    if ((row % 3 == j) && column % 4 == 1) {
      wattron(menu_w, A_STANDOUT);
      mvwaddch(menu_w, 9, XMAX / 2 - 2 + 2 * j,
               sesh->rotors[sesh->r_pos[j]][sesh->r_set[j]]);
      wattroff(menu_w, A_STANDOUT);
    }
    else
      mvwaddch(menu_w, 9, XMAX / 2 - 2 + 2 * j,
               sesh->rotors[sesh->r_pos[j]][sesh->r_set[j]]);
  }

  for (int j = 0; j < 10; j++) {
    if ((row % 10 == j) && (column % 4 == 2)) {
      wattron(menu_w, A_STANDOUT);
      mvwaddch(menu_w, 10, XMAX / 2 - 2 + 2 * j, sesh->plug_top[j]);
      wattron(menu_w,A_STANDOUT);
    }
    else
      mvwaddch(menu_w, 10, XMAX / 2 - 2 + 2 * j, sesh->plug_top[j]);
  }
  for (int j = 0; j < 10; j++) {
    if ((row % 10 == j) && (column % 4 == 2)) {
      wattron(menu_w, A_STANDOUT);
      mvwaddch(menu_w, 11, XMAX / 2 - 2 + 2 * j, sesh->plug_bot[j]);
      wattron(menu_w,A_STANDOUT);
    }
    else
      mvwaddch(menu_w, 11, XMAX / 2 - 2 + 2 * j, sesh->plug_bot[j]);
  }


  wrefresh(menu_w);
  refresh();
  return 1;
}

/*if ((!menu_w) || (!sesh_ptr) || (!(*sesh_ptr)))
    return NULL_INPUT;

  session_t *sesh = *sesh_ptr;
  char *headers[4] = {"Rotors: ", "Rotor settings: ", "Plugboard a: ",
                      "Plugboard b: "};
  int column = 0;
  int row = 0;
  int depth = 0; // 0 = column, 1 = row, 2 = selecting char.

  while (TRUE) {
    switch (depth) {
      case (0):
        for (int i = 0; i < 4; i++) {
          if (column % 4 == i) {
            wattron(menu_w, A_STANDOUT);
            mvwaddstr(menu_w, i + 8, XMAX / 2 - 10, headers[i]);
            mvwaddch(menu_w, i + 8, XMAX / 2 - 11, ACS_RARROW);
            wattroff(menu_w, A_STANDOUT);
          }
          else {
              mvwaddstr(menu_w, i + 8, XMAX / 2 - 10, headers[i]);
            mvwaddch(menu_w, i + 8, XMAX / 2 - 11, ACS_DIAMOND);
          }
        }

        wrefresh(menu_w);
        refresh();
        int action = getch();

        if (action == KEY_DOWN)
          column++;
        else if (action == KEY_UP)
          column--;
        else if (action == 27)
          return 0;
        else if (action == '\n')
          depth++;
        break; //case 0
 */
