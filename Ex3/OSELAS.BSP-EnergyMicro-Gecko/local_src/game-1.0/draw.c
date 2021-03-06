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
static uint16_t* frame;
static uint16_t current_color;



//Function declarations
void draw_to_display(int width, int height, int dx, int dy);
void draw_pixel(int x, int y, uint16_t color);
void draw_row(int row, uint16_t color);
int draw_init();
void draw_letter(int letter[7][5], int size, int x, int y, uint16_t color);
void draw_background_grid();
void draw_body_part(player *p);
uint16_t get_buffer_color(int x,int y);
void draw_frame();
void draw_background();
void player_win(player* p);

//Initialization function
int draw_init()
{
	printf("Hello Kjetil, I'm game! v12\n");

	
	current_color = white;
	

	fbfd = open(SCREEN_PATH, O_RDWR);
	printf("Open success\n");
	frame = (uint16_t*) mmap(NULL, SCREEN_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fbfd, 0);
	printf("Frame: %i\n", *frame);
	if (frame == MAP_FAILED){
		printf("Memory mapping failed!\n");
	}else{
		printf("Memory mapped successfully\n");
	}

	draw_background();
	draw_frame();
	printf("draw_frame done\n");
	
//	draw_background_grid(); //For debugging purposes
	
	draw_to_display(SCREEN_WIDTH, SCREEN_HEIGHT, 0, 0);
	printf("End of draw_init()");
	return 0;
}

void draw_to_display(int width, int height, int dx, int dy)
{
	rect.dx = dx;
	rect.dy = dy;
	rect.width = width;
	rect.height = height;

	int err = ioctl(fbfd, 0x4680, &rect);
	return 0;
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

void draw_body_part(player *p){
	int i, j;

	for(i = -1; i <= 1; i++){
		for (j = -1; j <= 1; j++){
			draw_pixel(p->head_x + j, p->head_y + i, p->color);
		}
	}
	//NORTH
	if (p->dir == NORTH){
			draw_pixel(p->head_x - 1, p->head_y + 2, p->color_dark);
			draw_pixel(p->head_x, p->head_y + 2, p->color_dark);
			draw_pixel(p->head_x + 1, p->head_y + 2, p->color_dark);
	}
	//SOUTH
	else if (p->dir == SOUTH){
			draw_pixel(p->head_x - 1, p->head_y - 2, p->color_dark);
			draw_pixel(p->head_x, p->head_y - 2, p->color_dark);
			draw_pixel(p->head_x + 1, p->head_y - 2, p->color_dark);
	}
	//WEST
	else if (p->dir == WEST){
			draw_pixel(p->head_x + 2, p->head_y - 1, p->color_dark);
			draw_pixel(p->head_x + 2, p->head_y, p->color_dark);
			draw_pixel(p->head_x + 2, p->head_y + 1, p->color_dark);
	}
	//EAST
	else if (p->dir == EAST){
			draw_pixel(p->head_x - 2, p->head_y - 1, p->color_dark);
			draw_pixel(p->head_x - 2, p->head_y, p->color_dark);
			draw_pixel(p->head_x - 2, p->head_y + 1, p->color_dark);
	}
	//Do not understand why (5, 5, x - 2, y - 2) does not work.
	draw_to_display(8, 8, p->head_x - 3, p->head_y - 3);
}

void draw_frame(){
	int i, j;
	for (i = 0; i < SCREEN_HEIGHT; i++){
		for (j = 0; j < SCREEN_WIDTH; j++){
			if (i < 2 || j < 2 || i > SCREEN_HEIGHT - 2 || j > SCREEN_WIDTH - 2){
				draw_pixel(j, i, frame_color);
			}
		}
	}
}

void draw_background_grid()
{
	int i, j;

	for (i = 0; i < SCREEN_HEIGHT; i++){
		for (j = 0; j < SCREEN_WIDTH; j++){
			if (i % 4 == 2 || j % 4 == 2 || i < 2 || j < 2 || i > SCREEN_HEIGHT - 4 || j > SCREEN_WIDTH - 4){
				draw_pixel(j, i, white);
			}
			else{
				draw_pixel(j, i, black);
			}
		}
	}
}

uint16_t get_buffer_color(int x,int y){
	return frame[y * SCREEN_WIDTH + x];
}


void draw_background(){
	
	int i, j;
	for (i = 0; i < (int)SCREEN_WIDTH; i++){
		for (j = 0; j < (int)SCREEN_HEIGHT; j++){
			draw_pixel(i, j, bg_color);
		}
	}


	current_color = text_color;

	draw_letter(_char_A, 1, 227, 115, current_color);
	draw_letter(_char_N, 1, 234, 115, current_color);
	draw_letter(_char_D, 1, 241, 115, current_color);
	draw_letter(_char_E, 1, 248, 115, current_color);
	draw_letter(_char_R, 1, 255, 115, current_color);
	draw_letter(_char_S, 1, 262, 115, current_color);
	draw_letter(_char_L, 1, 272, 115, current_color);
	draw_letter(_char_I, 1, 279, 115, current_color);
	draw_letter(_char_M, 1, 286, 115, current_color);
	draw_letter(_char_A, 1, 293, 115, current_color);

	draw_letter(_char_K, 1, 227, 125, current_color);
	draw_letter(_char_J, 1, 234, 125, current_color);
	draw_letter(_char_E, 1, 241, 125, current_color);
	draw_letter(_char_T, 1, 248, 125, current_color);
	draw_letter(_char_I, 1, 255, 125, current_color);
	draw_letter(_char_L, 1, 262, 125, current_color);
	draw_letter(_char_A, 1, 272, 125, current_color);
	draw_letter(_char_U, 1, 279, 125, current_color);
	draw_letter(_char_N, 1, 286, 125, current_color);
	draw_letter(_char_E, 1, 293, 125, current_color);
	
	draw_letter(_char_T, 1, 160, 220, current_color);
	draw_letter(_char_I, 1, 180, 220, current_color);
	draw_letter(_char_P, 1, 200, 220, current_color);
	draw_letter(_char_S, 1, 250, 220, current_color);
	draw_letter(_char_N, 1, 270, 220, current_color);
	draw_letter(_char_E, 1, 290, 220, current_color);
	draw_letter(_char_K, 1, 310, 220, current_color);
}

void player_win(player *p){
	current_color = p->color_dark;


	draw_letter(_char_P, 2, 100, 90, current_color);
	draw_letter(_char_L, 2, 114, 90, current_color);
	draw_letter(_char_A, 2, 128, 90, current_color);
	draw_letter(_char_Y, 2, 142, 90, current_color);
	draw_letter(_char_E, 2, 156, 90, current_color);
	draw_letter(_char_R, 2, 170, 90, current_color);

	if(current_color == green_dark){
		draw_letter(_char_one, 2, 190, 90, current_color);
	}
	else{
		draw_letter(_char_two, 2, 190, 90, current_color);
	}
	
	draw_letter(_char_W, 2, 210, 90, current_color);
	draw_letter(_char_I, 2, 224, 90, current_color);
	draw_letter(_char_N, 2, 238, 90, current_color);
	draw_letter(_char_S, 2, 252, 90, current_color);
	draw_to_display(152, 14, 100, 90);
}







