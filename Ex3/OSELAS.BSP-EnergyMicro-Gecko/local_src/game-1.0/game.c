#include <stdio.h>
#include <stdlib.h>
#include <time.h> // rand()
#include <signal.h>
#include <sys/types.h> // getpid()
#include <unistd.h>  //getpid()
#include <fcntl.h>  //F_SETOWN...
#include <time.h> //nanosleep()
//#include <linux/err.h>
#include "draw.h"
#include "buttons.h"


//Functions
int init_game();
int get_random_int(int min, int max);
int collides(int x, int y);
int move_player(player *p);
int turn_player(player *p);
int gamepad_init();
void gamepad_exit();
void input_handler(int sigio);
int setSnakeDir(uint8_t in);

//Type definitions




//Global variables
struct timespec *rem;
req->tv_nsec = 500;
player *p1, *p2;
int err, oflags, gp_err;
int f;
static uint8_t input_raw, state;

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
	printf("PREBODY\n");
	draw_body_part(p1);
	draw_body_part(p2);
	printf("POSTBODY\n");
	state = 0;
	gp_err = gamepad_init();
	printf("GPERR: %i\n",gp_err);
	int running = 1;
	int turn_err, rand2;
	turn t;
	int ctr = 0;
	while(true){
		const timespec *req = {tv_nsec = 500};
		if(nanosleep(req, rem) == 0){

			printf("p1 next: %i\n",	p1->next_turn);		
			if(p1->next_turn){
				turn_player(p1);
				p1->next_turn = 0;
			}else{
				move_player(p1);
			}

			printf("p1 next: %i\n",	p2->next_turn);
			if(p2->next_turn){
				turn_player(p2);
				p2->next_turn = 0;
			}else{
				move_player(p2);
			}
			if (collides(p1->head_x, p1->head_y)){
				p1->color = pink;
				draw_body_part(p1);
				exit(EXIT_SUCCESS);
			}
			if (collides(p2->head_x, p2->head_y)){
				p2->color = pink;
				draw_body_part(p2);
				exit(EXIT_SUCCESS);
			}
			draw_body_part(p1);
			draw_body_part(p2);
		}
		else{
			const timespec *reqt = {tv_nsec = rem->tv_nsec};
			nanosleep(reqt, rem);		
		}
	}

	exit(EXIT_SUCCESS);
}



int gamepad_init(){
	
	f = open("/dev/gamepad", O_RDONLY | O_NONBLOCK);
	if (!f){
		printf("ERROR: Unable to open gamepad device!\n");
		return -1;
	}
	if (signal(SIGIO, &input_handler) == SIG_ERR){
		printf("Could not register device...\n");
		return -1;
	}

	if (fcntl(f, F_SETOWN, getpid()) == -1){
		printf("Could not be set as owner..\n");
		return -1;
	}

	oflags = fcntl(f, F_GETFL) | FASYNC;

	if(fcntl(f, F_SETFL, oflags) == -1){
		printf("Could not set flag...\n");
		return -1;
	}

	if (fcntl(f, F_SETOWN, getpid()) == -1){
		printf("Could not be set as owner..\n");
		return -1;
	}

	return 0;
}

void gamepad_exit(){
	close(f);
}

void input_handler(int sigio){
	
	printf("Input Handler %i\n", sigio);

	//unsigned int buff[32];

	int cnt = read(f, (void*)&input_raw, sizeof(uint8_t));

	printf("Buffer value: %i\n", input_raw);

	state = ~state & (~input_raw);

	setSnakeDir(state);
	printf("New state: %i\n", state);
	state = 0;


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
	int random = ((rand() % (max - min)) + min);
	//printf("Random number: %i\n", random);
	return random;
}

int move_player(player *p){
	switch(p->dir){
		case(NORTH):
			p->head_y-=4;
			break;
		case(SOUTH):
			p->head_y+=4;
			break;
		case(EAST):
			p->head_x+=4;
			break;
		case(WEST):
			p->head_x-=4;
			break;
	}
}
int turn_player(player *p){ // Might need to update tail_x/y
	int t = p->next_turn;
	printf("Turning:%i, %i\n",p->dir,t); 
	if (p->dir == EAST && t == RIGHT){
		if (collides(p->head_x, p->head_y + 4)){
			printf("COLLIDES!!!");
			return -1;
		}
		else{
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
			p->head_x += 4;
			p->dir = EAST;
		}
	}
	draw_body_part(p);
	return 0;

}

int setSnakeDir(uint8_t in)
{
	printf("SnakeDir in: %i\n", in);
	if(in & sw1){
		p1->next_turn = 1;
	}else if(in & sw4){
		p1->next_turn = 2;
	}else if(in & sw8){
		p2->next_turn = 1;
	}else if(in & sw7){
		p2->next_turn = 2;
	}
	return 0;
}



int collides(int x, int y){
	return get_buffer_color(x, y) != bg_color;
}




