#include <stdio.h>
#include <stdlib.h>
#include <time.h> // rand()

#include "draw.h"


//Functions
int init_game();
int get_random_int(int min, int max);

//Type definitions




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
	draw_body_part(p1->head_x, p1->head_y, green);

	exit(EXIT_SUCCESS);
}


int init_game(){
	// *p1 = (struct tuple){get_random_int(3, SCREEN_WIDTH / 2), get_random_int(3, SCREEN_HEIGHT / 2), 0, 0};


	//Instatiate Player1
	p1 = (tuple*) malloc(sizeof(tuple));
	if (p1 == NULL) {
		// Something went wrong
		return 0;
	}
	p1->head_x = get_random_int(3, SCREEN_WIDTH / 2);
	p1->head_y = get_random_int(3, SCREEN_HEIGHT / 2);


	//printf("HEAD X SHOULD BE RANDOM..%i\n AND TAIL X SHOULD BE AS RANDOM..%i\n", p1->head_x, p1->tail_x);
	


	switch (p1->head_x % 4){
		case 3:
			p1->head_x++;
			break;
		case 1:
			p1->head_x--;
			break;
		case 2:
			p1->head_x = p1->head_x + 2;
			break;
	}

	switch (p1->head_y % 4){
		case 3:
			p1->head_y++;
			break;
		case 1:
			p1->head_y--;
			break;
		case 2:
			p1->head_y = p1->head_y + 2;
			break;
	}

	p1->tail_x = p1->head_x;
	p1->tail_y = p1->head_y;


	//Instatiate Player2
	p2 = (tuple*) malloc(sizeof(tuple));

	if (p2 == NULL){
		return 0;
	}

	p2->head_x = get_random_int(SCREEN_WIDTH / 2, SCREEN_WIDTH - 3);
	p2->head_y = get_random_int(SCREEN_HEIGHT / 2, SCREEN_HEIGHT - 3);

	switch (p2->head_x % 4){
		case 3:
			p2->head_x++;
			break;
		case 1:
			p2->head_x--;
			break;
		case 2:
			p2->head_x = p2->head_x + 2;
			break;
	}

	switch (p2->head_y % 4){
		case 3:
			p2->head_y++;
			break;
		case 1:
			p2->head_y--;
			break;
		case 2:
			p2->head_y = p2->head_y + 2;
			break;
	}

	p2->tail_x = p2->head_x;
	p2->tail_y = p2->head_y;

	return 1;

}

int get_random_int(int min, int max){
	srand(time(NULL));
	int random = ((rand() % max) + min);
	//printf("Random number: %i\n", random);
	return random;
}

