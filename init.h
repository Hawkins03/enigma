#ifndef INIT_H
#define INIT_H

#define NUM_ROTORS      (3)
#define NUM_LETTERS     (26)

#define STARTUP_ERR     (-1)
#define READ_ERR        (-2)
#define WRITE_ERR       (-3)
#define NULL_INPUT      (-4)

typedef struct session_struct {
  char rotors[4][27];
  int r_pos[3];
  int r_set[3];
  unsigned char plug_top[10];
  unsigned char plug_bot[10];
} session_t;


/* Function Prototypes */
void print_settings(session_t *);
session_t *get_settings();
int set_settings(session_t *);

#endif /* INIT_H */
