#define NUM_ROTORS      (3)
#define NUM_LETTERS     (26)

#define STARTUP_ERR     (-1)
#define READ_ERR        (-2)
#define WRITE_ERR       (-3)
#define NULL_INPUT      (-4)

typedef struct settings_struct {
  char rotors[4][27];
  int r_pos[3];
  int r_set[3];
  unsigned char plug_top[10];
  unsigned char plug_bot[10];
} settings_struct_t;

/* Function Prototypes */
void print_settings(settings_struct_t *);
settings_struct_t *get_settings();
int set_settings(settings_struct_t *settings);
