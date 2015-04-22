#include <stdint.h>

//Draws the framebuffer
void draw_to_display(int width, int height, int dx, int dy);

//Draws a single pixel
void draw_pixel(int x, int y, uint16_t color);

//Draws a row
void draw_row(int row, uint16_t color);

//Initializes drawing
void draw_init();

//Draw a letter
void draw_letter(int letter[7][5], int size, int x, int y, uint16_t color);

//Draw background grid
void draw_background_grid();

//Draws bodypart of snake
void draw_body_part(int x, int y, uint16_t color);

//System constants
#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240
#define SCREEN_SIZE SCREEN_WIDTH * SCREEN_HEIGHT * sizeof(uint16_t)
#define SCREEN_PATH "/dev/fb0"


typedef struct player {
	int head_x;
	int head_y;
	int tail_x;
	int tail_y;
} player;

typedef enum {LEFT = 0, RIGHT = 1} direction;

static uint16_t red = (31 << 11) + (0 << 5) + (0 << 0);
static uint16_t green = (0 << 11) + (63 << 5) + (0 << 0);
static uint16_t blue = (0 << 11) + (0 << 5) + (31 << 0);
static uint16_t pink = (30 << 11) + (36 << 5) + (31 << 0);
static uint16_t black = (0 << 11) + (0 << 5) + (0 << 0);
static uint16_t white = (31 << 11) + (63 << 5) + (31 << 0);