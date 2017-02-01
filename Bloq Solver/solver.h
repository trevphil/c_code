
#define NUM_ROWS 4
#define NUM_COLS 4
#define NUM_COLORS 4
// KEY_LENGTH = NUM_ROWS * NUM_COLS + 1
#define KEY_LENGTH 17
#define MOVE_LEN 8
#define KEY_COUNT (2048 * 1024)
#define EMPTY '.'
#define OOB '!'

char * solve(char *initial);