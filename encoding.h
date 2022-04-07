#ifndef ENCODING_H
#define ENCODING_H

#define NUM_ROTORS      (3)
#define NUM_LETTERS     (26)

#define STARTUP_ERR     (-1)
#define READ_ERR        (-2)
#define NULL_INPUT      (-3)

typedef struct settings_struct {
  unsigned int r_pos[3];
  unsigned int r_set[3];
  unsigned char plug_top[10];
  unsigned char plug_bot[10];
} settings_struct_t;

/* Function Prototypes */
void print_settings(settings_struct_t *);
settings_struct_t *open_file(char *in_file);

#endif
