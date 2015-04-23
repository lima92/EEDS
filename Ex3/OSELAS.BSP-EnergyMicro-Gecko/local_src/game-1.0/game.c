#include <stdio.h>
#include <stdlib.h>
#include <time.h> // rand()
#include <signal.h>
#include <sys/types.h> // getpid()
#include <unistd.h>  //getpid()
#include <fcntl.h>  //F_SETOWN...
#include "draw.h"


//Functions
int init_game();
int get_random_int(int min, int max);
int collides(int x, int y);
int turn_player(player *p, turn t);
int gamepad_init();
void gamepad_exit();
void input_handler(int sigio);

//Type definitions




//Global variables
player *p1, *p2;
int err, oflags;
FILE *f;

int main(int argc, char *argv[])
{
	srand(time(NULL));

	err = draw_init();
	if (err){
		printf("failed to draw_init()");
	}else{
		printf("returned from draw_init()");
	}
	err = init_game();
	if (err == -1){
		printf("Could not initialize game. Exit...");
		exit(EXIT_SUCCESS);
	}
	printf("PREBODY");
	draw_body_part(p1->head_x, p1->head_y, p1->color);
	draw_body_part(p2->head_x, p2->head_y, p2->color);
	printf("POSTBODY");
	gamepad_init();
	int running = 1;
	int turn_err, rand2;
	turn t;
	int ctr = 0;
	while(ctr < 100){
		ctr++;
		if(ctr % 5 == 0){
			rand2 = get_random_int(0,2);
			turn_err = turn_player(p1, rand2);
			if(turn_err == -1){
				break;
			}
		}
		else{
			switch(p1->dir){
				case(NORTH):
					p1->head_y-=4;
					break;
				case(SOUTH):
					p1->head_y+=4;
					break;
				case(EAST):
					p1->head_x+=4;
					break;
				case(WEST):
					p1->head_x-=4;
					break;
			}
		}

		draw_body_part(p1->head_x, p1->head_y, p1->color);
	}

	exit(EXIT_SUCCESS);
}



int gamepad_init(){
	
	f = fopen(SCREEN_PATH, "r");
	if (f){
		printf("Unable to open gamepad device!\n");
		return -1;
	}
	signal(SIGIO, &input_handler);
	fcntl(fileno(f), F_SETOWN, getpid());

	oflags = fcntl(fileno(f), F_GETFL);

	fcntl(fileno(f), F_SETFL, oflags | FASYNC);

	return 0;
}

void gamepad_exit(){
	fclose(f);
}

void input_handler(int sigio){
	
	printf("Input Handler %i\n", sigio);

	unsigned int buff[32];

	fread(buff, 1, 1, f);

	printf("Buffer value: %i\n", buff);
}

int init_game()
{
	//Instatiate Player1
	p1 = (player*) malloc(sizeof(player));
	if (p1 == NULL) {
		// Something went wrong
		return -1;
	}
	p1->head_x = get_random_int(3, SCREEN_WIDTH / 2);
	p1->head_y = get_random_int(3, SCREEN_HEIGHT / 2);
	p1->dir = EAST;
	p1->color = green;


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
	p2 = (player*) malloc(sizeof(player));

	if (p2 == NULL){
		return -1;
	}

	p2->head_x = get_random_int(SCREEN_WIDTH / 2, SCREEN_WIDTH - 3);
	p2->head_y = get_random_int(SCREEN_HEIGHT / 2, SCREEN_HEIGHT - 3);
	p2->dir = WEST;
	p2->color = red;

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

	return 0;

}

int get_random_int(int min, int max){
	int random = ((rand() % max) + min);
	//printf("Random number: %i\n", random);
	return random;
}


int turn_player(player *p, turn t){ // Might need to update tail_x/y

	printf("Turning:%i, %i\n",p->dir,t); 
	if (p->dir == EAST && t == RIGHT){
		if (collides(p->head_x, p->head_y + 4)){
			printf("COLLIDES!!!");
			return -1;
		}
		else{
			printf("Head_x %i\nHead_y %i\nHead_x - 1 %i\nHead_y + 2 %i\n", p->head_x, p->head_y, p->head_x - 1, p->head_y + 2);
			draw_pixel(p->head_x - 1, p->head_y + 2, p->color);
			draw_pixel(p->head_x, p->head_y + 2, p->color);
			draw_pixel(p->head_x + 1, p->head_y + 2, p->color);
			p->head_y += 4;
			p->dir = SOUTH;
		}
	}
	else if (p->dir == EAST && t == LEFT){
		if (collides(p->head_x, p->head_y - 4)){
			printf("COLLIDES!!");
			return -1;
		}
		else{
			draw_pixel(p->head_x - 1, p->head_y - 2, p->color);
			draw_pixel(p->head_x, p->head_y - 2, p->color);
			draw_pixel(p->head_x + 1, p->head_y - 2, p->color);
			p->head_y -= 4;
			p->dir = NORTH;
		}
	}

	else if (p->dir == WEST && t == RIGHT){
		if (collides(p->head_x, p->head_y - 4)){
			printf("COLLIDES!!!");
			return -1;
		}
		else{
			draw_pixel(p->head_x - 1, p->head_y - 2, p->color);
			draw_pixel(p->head_x, p->head_y - 2, p->color);
			draw_pixel(p->head_x + 1, p->head_y - 2, p->color);
			p->head_y -= 4;
			p->dir = NORTH;
		}
	}
	else if (p->dir == WEST && t == LEFT){
		if (collides(p->head_x, p->head_y + 4)){
			printf("COLLIDES!!");
			return -1;
		}
		else{
			draw_pixel(p->head_x - 1, p->head_y + 2, p->color);
			draw_pixel(p->head_x, p->head_y + 2, p->color);
			draw_pixel(p->head_x + 1, p->head_y + 2, p->color);
			p->head_y += 4;
			p->dir = SOUTH;
		}
	}

	else if (p->dir == NORTH && t == RIGHT){
		if (collides(p->head_x + 4, p->head_y)){
			printf("COLLIDES!!!");
			return -1;
		}
		else{
			draw_pixel(p->head_x + 2, p->head_y - 1, p->color);
			draw_pixel(p->head_x + 2, p->head_y, p->color);
			draw_pixel(p->head_x + 2, p->head_y + 1, p->color);
			p->head_x += 4;
			p->dir = EAST;
		}
	}
	else if (p->dir == NORTH && t == LEFT){
		if (collides(p->head_x - 4, p->head_y)){
			printf("COLLIDES!!");
			return -1;
		}
		else{
			draw_pixel(p->head_x - 2, p->head_y - 1, p->color);
			draw_pixel(p->head_x - 2, p->head_y, p->color);
			draw_pixel(p->head_x - 2, p->head_y + 1, p->color);
			p->head_x -= 4;
			p->dir = WEST;
		}
	}

	else if (p->dir == SOUTH && t == RIGHT){
		if (collides(p->head_x - 4, p->head_y)){
			printf("COLLIDES!!!");
			return -1;
		}
		else{
			draw_pixel(p->head_x - 2, p->head_y - 1, p->color);
			draw_pixel(p->head_x - 2, p->head_y, p->color);
			draw_pixel(p->head_x - 2, p->head_y + 1, p->color);
			p->head_x -= 4;
			p->dir = WEST;
		}
	}
	else if (p->dir == SOUTH && t == LEFT){
		if (collides(p->head_x + 4, p->head_y)){
			printf("COLLIDES!!");
			return -1;
		}
		else{
			draw_pixel(p->head_x + 2, p->head_y - 1, p->color);
			draw_pixel(p->head_x + 2, p->head_y, p->color);
			draw_pixel(p->head_x + 2, p->head_y + 1, p->color);
			p->head_x += 4;
			p->dir = EAST;
		}
	}
	return 0;

}



int collides(int x, int y){
	return get_buffer_color(x, y) != bg_color;
}




