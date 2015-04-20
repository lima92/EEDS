#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>		// uint16_t
#include <linux/fb.h>	// framebuffer
#include <sys/mman.h>	// mmap()
#include <sys/ioctl.h> 	// ioctl()
#include <fcntl.h> 		// open()
#include "draw.h"		// Header file
#include "fonts.h"		// Font arrays

//System constants
#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240
#define SCREEN_SIZE SCREEN_WIDTH * SCREEN_HEIGHT * sizeof(uint16_t)
#define SCREEN_PATH "/dev/fb0"

//Global variables
static struct fb_copyarea rect;
static int fbfd;
static uint16_t* frame;
static uint16_t current_color;
static uint16_t red = (31 << 11) + (0 << 5) + (0 << 0);
static uint16_t green = (0 << 11) + (63 << 5) + (0 << 0);
static uint16_t blue = (0 << 11) + (0 << 5) + (31 << 0);
static uint16_t pink = (30 << 11) + (36 << 5) + (31 << 0);
static uint16_t black = (0 << 11) + (0 << 5) + (0 << 0);
static uint16_t white = (31 << 11) + (63 << 5) + (31 << 0);

//Function declarations
void draw_to_display(void);
void draw_pixel(int x, int y, uint16_t color);
void draw_row(int row, uint16_t color);
void draw_init();
void draw_letter(int letter[7][5], int size, int x, int y, uint16_t color);


//Initialization function
void draw_init()
{
	printf("Hello Kjetil, I'm game!\n");

	
	current_color = white;
	

	fbfd = open(SCREEN_PATH, O_RDWR);

	frame = (uint16_t*) mmap(NULL, SCREEN_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fbfd, 0);

	if (frame == MAP_FAILED){
		printf("Memory mapping failed!");
	}
	
	int i, j;
	for (i = 0; i < (int)SCREEN_WIDTH; i++){
		for (j = 0; j < (int)SCREEN_HEIGHT; j++){
			draw_pixel(i, j, current_color);
		}
	}
	current_color = black;
	draw_letter(_char_B, 5, 0, 0, current_color);

	draw_background_grid();

	draw_to_display();

	exit(EXIT_SUCCESS);
}

void draw_to_display(void)
{
	rect.dx = 0;
	rect.dy = 0;
	rect.width = SCREEN_WIDTH;
	rect.height = SCREEN_HEIGHT;

	ioctl(fbfd, 0x4680, &rect);
}

void draw_pixel(int x, int y, uint16_t color)
{
	frame[y * SCREEN_WIDTH + x] = color;
}

void draw_row(int row, uint16_t color)
{
	int i;
	for (i = 0; i < SCREEN_WIDTH; i++){
		draw_pixel(i, row, color);
	}
}

void draw_letter(int letter[7][5], int size, int x, int y, uint16_t color)
{
	int i, j, ctrX, ctrY, k, l;
	ctrY = 0;
	for (i = y; i < y + size * 7; i+=size){
		ctrX = 0;
		ctrY++;
		for (j = x; j < x + size * 5; j+=size){
			if (letter[ctrY][ctrX] == 1){
				for (k = 0; k < size; k++){
					for (l = 0; l < size; l++){
						draw_pixel(j + l, i + k, color);
					}
				}
			}
			ctrX++;
		}
	}
}

void draw_background_grid()
{
	int i, j;

	for (i = 0; i < SCREEN_WIDTH; i++){
		for (j = 0; j < SCREEN_HEIGHT; j++){
			if (i % 4 == 0 || j % 4 == 0){
				draw_pixel(j, i, white);
			}
			else{
				draw_pixel(j, i, black);
			}
		}
	}
}










