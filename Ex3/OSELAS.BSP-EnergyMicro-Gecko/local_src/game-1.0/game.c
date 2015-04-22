#include <stdio.h>
#include <stdlib.h>
#include <time.h> // rand()

#include "draw.h"


//Functions
int init_game();
int get_random_int(int min, int max);

//Type definitions
typedef enum {LEFT = 0, RIGHT = 1} direction;

typedef struct tuple {
	int head_x;
	int head_y;
	int tail_x;
	int tail_y;
} tuple;

//Global variables
tuple *p1, *p2;
int err;

int main(int argc, char *argv[])
{
	draw_init();
	err = init_game();
	if (err == 0){
		printf("Could not initialize game. Exit...");
		exit(EXIT_SUCCESS);
	}

	exit(EXIT_SUCCESS);
}


int init_game(){
	//*p1 = (struct tuple){get_random_int(3, SCREEN_WIDTH / 2), get_random_int(3, SCREEN_HEIGHT / 2), 0, 0};

	p1 = (tuple*) malloc(sizeof(tuple));
	if (p1 == NULL) {
		// Something went wrong
		return 0;
	}
	p1->head_x = get_random_int(3, SCREEN_WIDTH / 2);
	p1->head_y = get_random_int(3, SCREEN_HEIGHT / 2);
	p1->tail_x = p1->head_x;
	p1->tail_y = p1->head_y;

	printf("HEAD X SHOULD BE RANDOM..%i\n AND TAIL X SHOULD BE AS RANDOM..%i\n", p1->head_x, p1->tail_x);
	return 1;
	/*if (p1->head_x % 4 == 2){
		p1->head_x++;
	}
	if (p1->head_y % 4 == 2){
		p1->head_y++;
	}
	p1->tail_x = p1->head_x;
	p1->tail_y = p1->head_y;
	printf("Head_X %i \n" ,p1->head_x);
	printf("Tail_X %i \n" ,p1->tail_x);

	*p2 = (struct tuple){get_random_int(SCREEN_WIDTH / 2, SCREEN_WIDTH - 3), get_random_int(SCREEN_HEIGHT / 2, SCREEN_HEIGHT - 3), 0, 0};
	if (p2->head_x % 4 == 2){
		p2->head_x--;
	}
	if (p2->head_y % 4 == 2){
		p2->head_y--;
	}
	p2->tail_x = p2->head_x;
	p2->tail_y = p2->head_y;*/
}

int get_random_int(int min, int max){
	printf("INIT V1");
	srand(time(NULL));
	int random = ((rand() % max) + min);
	printf("Random number: %i\n", random);
	return random;
}

