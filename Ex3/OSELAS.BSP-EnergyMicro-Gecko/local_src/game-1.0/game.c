#include <stdio.h>
#include <stdlib.h>
#include <time.h> // rand()

#include "draw.h"


//Functions
void init_game();
int get_random_int(int min, int max);

//Type definitions
typedef enum {LEFT = 0, RIGHT = 1} directions;

typedef struct tuple{
	int x;
	int y;
};

//Global variables
tuple p1_start_position, p2_start_position;

int main(int argc, char *argv[])
{
	draw_init();
	init_game();

	exit(EXIT_SUCCESS);
}


void init_game(){
	p1_start_position = {get_random_int(3, SCREEN_WIDTH / 2), get_random_int(3, SCREEN_HEIGHT / 2)};

	if (p1_start_position.x % 4 == 2){
		p1_start_position.x++;
	}
	if (p1_start_position.y % 4 == 2){
		p1_start_position.y++;
	}

	p2_start_position = {get_random_int(SCREEN_WIDTH / 2, SCREEN_WIDTH - 3), get_random_int(SCREEN_HEIGHT / 2, SCREEN_HEIGHT - 3)};

	if (p2_start_position.x % 4 == 2){
		p2_start_position.x--;
	}
	if (p2_start_position.y % 4 == 2){
		p2_start_position.y--;
	}
}

int get_random_int(int min, int max){
	srand(time(NULL));
	return ((rand() % max) + min);
}

