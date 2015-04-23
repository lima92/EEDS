#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>		// uint16_t
#include <linux/fb.h>	// framebuffer
#include <sys/mman.h>	// mmap()
#include <sys/ioctl.h> 	// ioctl()
#include <fcntl.h> 		// open()
#include "draw.h"		// Header file
#include "fonts.h"		// Font arrays



//Global variables
static struct fb_copyarea rect;
static int fbfd;
//static uint16_t* frame;
static uint16_t current_color;



//Function declarations
void draw_to_display(int width, int height, int dx, int dy);
void draw_pixel(int x, int y, uint16_t color);
void draw_row(int row, uint16_t color);
void draw_init();
void draw_letter(int letter[7][5], int size, int x, int y, uint16_t color);
void draw_background_grid();
void draw_body_part(int x, int y, uint16_t color);

//Initialization function
void draw_init()
{
	printf("Hello Kjetil, I'm game! v10\n");

	
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
//	draw_background_grid();
	
	draw_letter(_char_A, 1, 100, 100, current_color);
	draw_letter(_char_N, 1, 107, 100, current_color);
	draw_letter(_char_D, 1, 114, 100, current_color);
	draw_letter(_char_E, 1, 121, 100, current_color);
	draw_letter(_char_R, 1, 128, 100, current_color);
	draw_letter(_char_S, 1, 135, 100, current_color);
	draw_letter(_char_L, 1, 145, 100, current_color);
	draw_letter(_char_I, 1, 152, 100, current_color);
	draw_letter(_char_M, 1, 159, 100, current_color);
	draw_letter(_char_A, 1, 166, 100, current_color);

	draw_letter(_char_K, 1, 100, 110, current_color);
	draw_letter(_char_J, 1, 107, 110, current_color);
	draw_letter(_char_E, 1, 114, 110, current_color);
	draw_letter(_char_T, 1, 121, 110, current_color);
	draw_letter(_char_I, 1, 128, 110, current_color);
	draw_letter(_char_L, 1, 135, 110, current_color);
	draw_letter(_char_A, 1, 145, 110, current_color);
	draw_letter(_char_U, 1, 152, 110, current_color);
	draw_letter(_char_N, 1, 159, 110, current_color);
	draw_letter(_char_E, 1, 166, 110, current_color);
	
	draw_letter(_char_T, 1, 100, 200, current_color);
	draw_letter(_char_I, 1, 130, 200, current_color);
	draw_letter(_char_P, 1, 160, 200, current_color);
	draw_letter(_char_K, 1, 230, 200, current_color);
	draw_letter(_char_E, 1, 260, 200, current_color);
	draw_letter(_char_K, 1, 290, 200, current_color);
	draw_to_display(SCREEN_WIDTH, SCREEN_HEIGHT, 0, 0);

	printf("HEREWEGO\n");
	FILE *f;
	f = fopen("/dev/gamepad", "r");
	printf("OPENED GAMEPAD!\n");

	unsigned int buff[32];

	if(f == NULL){
		perror("Errororor");
	}		
	printf("fread");
	
	fread(buff, 1, 1, f);

	printf("buff %s\n", buff);
	
	fclose(f);
	printf("Done");
	

}

void draw_to_display(int width, int height, int dx, int dy)
{
	rect.dx = dx;
	rect.dy = dy;
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
		ctrY++;
	}
}

void draw_body_part(int x, int y, uint16_t color){
	int i, j;

	for(i = -1; i <= 1; i++){
		for (j = -1; j <= 1; j++){
			draw_pixel(x + j, y + i, color);
		}
	}
	draw_to_display(3, 3, x - 1, y - 1);
}


void draw_background_grid()
{
	int i, j;

	for (i = 0; i < SCREEN_HEIGHT; i++){
		for (j = 0; j < SCREEN_WIDTH; j++){
			if (i % 4 == 2 || j % 4 == 2 || i < 2 || j < 2 || i > SCREEN_HEIGHT - 2 || j > SCREEN_WIDTH - 2){
				draw_pixel(j, i, white);
			}
			else{
				draw_pixel(j, i, black);
			}
		}
	}
}



