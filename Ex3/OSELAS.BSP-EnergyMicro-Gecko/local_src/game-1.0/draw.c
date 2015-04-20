#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <linux/fb.h>	//framebuffer
#include <sys/mman.h>	// mmap()
#include <sys/ioctl.h> 	// ioctl()
#include <fcntl.h> 		// open()
#include "draw.h"

#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240
#define SCREEN_SIZE SCREEN_WIDTH * SCREEN_HEIGHT * sizeof(uint16_t)
#define SCREEN_PATH "/dev/fb0"


static struct fb_copyarea rect;
static int fbfd;
static uint16_t* frame;
static uint16_t current_color;

//Function declarations
void draw_to_display(void);
void draw_pixel(int x, int y, uint16_t color);
void draw_row(int row, uint16_t color);
void draw_init();

void draw_init()
{
	printf("Hello Kjetil, I'm game!\n");

	
	current_color = (31 << 11) + (63 << 5) + (31 << 0);
	

	fbfd = open(SCREEN_PATH, O_RDWR);

	frame = (uint16_t*) mmap(NULL, SCREEN_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fbfd, 0);

	if (frame == MAP_FAILED){
		printf("Memory mapping failed!");
	}
	
	int i, j;
	for (i = 0; i < (int)SCREEN_WIDTH; i++){
		for (j = 0; j < (int)SCREEN_HEIGHT; j++){
			frame[i + j * SCREEN_WIDTH] = current_color;
		}
	}

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

void draw_pixel(int x, int y, uint16_t color){
	frame[y * SCREEN_WIDTH + x] = color;
}

void draw_row(int row, uint16_t color){
	int i;
	for (i = 0; i < SCREEN_WIDTH; i++){
		draw_pixel(i, row, color);
	}
}




