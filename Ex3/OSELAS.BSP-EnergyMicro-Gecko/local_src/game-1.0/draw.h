#include <stdint.h>

//Draws the framebuffer
void draw_to_display(void);

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

//System constants
#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240
#define SCREEN_SIZE SCREEN_WIDTH * SCREEN_HEIGHT * sizeof(uint16_t)
#define SCREEN_PATH "/dev/fb0"