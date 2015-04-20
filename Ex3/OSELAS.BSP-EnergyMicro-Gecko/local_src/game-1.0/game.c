#include <stdio.h>
#include <stdlib.h>
#include <linux/fb.h>
#include <sys/mman.h>

#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240

typedef struct RGB {
	unsigned int red	: 5 ;
	unsigned int green	: 6 ;
	unsigned int blue	: 5 ;
} RGB;

typedef struct fb_copyarea rect;

int main(int argc, char *argv[])
{
	printf("Hello Kjetil, I'm game!\n");

	RGB red = {0b11111, 0b000000, 0b00000};

	RGB frame[SCREEN_WIDTH][SCREEN_HEIGHT];

	rect.dx = x;
	rect.dy = y;
	rect.width = SCREEN_WIDTH;
	rect.height = SCREEN_HEIGHT;

	for (int i = 0; i < SCREEN_WIDTH; i++){
		for (int j = 0; j < SCREEN_HEIGHT; j++){
			frame[i][j] = red;
		}
	}

	mmap(&frame, SCREEN_HEIGHT*SCREEN_WIDTH*2, PROT_WRITE, MAP_SHARED, fbfd, sysconf(_SC_PAGE_SIZE));

	ioctl(fbfd, 0x4680, &rect);

	exit(EXIT_SUCCESS);
}
