#include "state_machine.h"
#include "log.h"
#include "display.h"
#include "delay.h"
#include "led.h"
#include "snake_list.h"
#include "switch.h"
#include <stdlib.h>


uint8_t flags;
uint8_t x;
uint8_t y;
uint8_t prev_x;
uint8_t prev_y;
int direction;
int num_points;
Node * snake_head;
int length_of_snake;
uint8_t goal_x;
uint8_t goal_y;

void generate_goal(uint8_t *x, uint8_t *y){
	*x = 2 + rand() % (SCREEN_HEIGHT - 3);
	*y = 2 + rand() % (SCREEN_WIDTH - 3);
}

void reset_game() {
	LOG("Starting Game\r\n");
	x = 0;
	y = 0;
	prev_x = 0;
	prev_y = 0;
	direction = UP;
	num_points = 0;
	length_of_snake = 5;
	goal_x = START_GOAL_X;
	goal_y = START_GOAL_Y;
	set_pixel(goal_x, goal_y, 1);
	snake_head = add_to_snake_head(x, y);
	clear_flags();
}
void process_goal() {
	LOG("Goal Scored\r\n");
	num_points += 1;
	length_of_snake += 3;
	set_pixel(goal_x, goal_y, 0);
	generate_goal(&goal_x, &goal_y);
	while(goal_x < 2 || goal_x > SCREEN_HEIGHT -2 || goal_y < 2 || goal_y > SCREEN_WIDTH -2){ //Regen goal if out of range
		generate_goal(&goal_x, &goal_y);
	}
	LOG("New Goal %i %i\r\n", goal_x, goal_y);
	set_pixel(goal_x, goal_y, 1);
	push_display();
	handle_event(SCORE_COMP);
}
void end_game() {
	LOG("You Died\r\n");
	delete_snake_list();
	free(snake_head);
	snake_head = NULL;
	clear_display();
	delay_ms(100);
	fail_screen();
	delay_ms(1000);
	clear_display();
	handle_event(START);
}

void do_nothing() {

}


Transition state_table[] = {
    {GAME_START,    START,		GAME_ACTIVE, 	reset_game},
    {GAME_ACTIVE, 	SCORE,   	GOAL_SCORED, 	process_goal},
	{GOAL_SCORED, 	SCORE_COMP, GAME_ACTIVE, 	do_nothing},
    {GAME_ACTIVE,   FAIL, 		GAME_ENDED,  	end_game},
	{GOAL_SCORED,   FAIL, 		GAME_ENDED,  	end_game},
	{GAME_ENDED,   	START, 		GAME_ACTIVE, 	reset_game}
};

static State current_state = GAME_START;


void handle_event(Event event){
	//LOG("Current_state %i, Handling event %i\r\n", current_state, event);
	int event_found = 0;
	for (int i = 0; i < sizeof(state_table)/sizeof(Transition); i++) {
		if (!event_found && state_table[i].current_state == current_state && state_table[i].event == event) {

			current_state = state_table[i].next_state;
			state_table[i].action();
			event_found = 1;
			break;
		}
	}

	if(event_found == 0){
		printf("No valid transition.\r\n");
	}
}

int within_goal_range(uint8_t x, uint8_t y){
	return (abs(x - goal_x) < 2) && (abs(y - goal_y) < 2);
}



int start_game(){
	handle_event(START);
	return 0;
}

void update_sate_machine(){

	if(current_state == GAME_ACTIVE && !within_goal_range(snake_head->x, snake_head->y)){

		if(length_of_snake_list() > length_of_snake){
			remove_from_snake_tail(&prev_x, &prev_y);
		}
		set_pixel(snake_head->x, snake_head->y, 1);
		set_pixel(prev_x, prev_y, 0);
		push_display();

		flags = get_flags();
		if(flags) {
			if (flags & 0x01 && direction != DOWN){ //Up switch
				direction = UP;
			} else if (flags & 0x02 && direction != LEFT){ //Right switch
				direction = RIGHT;
			} else if (flags & 0x04 && direction != UP){ //Down switch
				direction = DOWN;
			} else if (flags & 0x08 && direction != RIGHT){ //Left switch
				direction = LEFT;
			}
			clear_flags();
		}

		switch (direction){
			case UP: {
				if(y < SCREEN_WIDTH -1){
					y +=1;
					snake_head = add_to_snake_head(x, y);
				} else {
					handle_event(FAIL);
				}
				break;
			}
			case RIGHT: {
				if( x < SCREEN_HEIGHT -1){
					x +=1;
					snake_head = add_to_snake_head(x, y);
				} else {
					handle_event(FAIL);
				}
				break;
			}
			case  DOWN: {
				if (y > 1){
					y -=1;
					snake_head = add_to_snake_head(x, y);
				} else {
					handle_event(FAIL);
				}
				break;
			}
			case LEFT: {
				if(x > 1){
					x -=1;
					snake_head = add_to_snake_head(x, y);
				}	else {
					handle_event(FAIL);
				}
				break;
			}
			default:
				LOG("No direction specified\r\n");
		}

	} else if(current_state == GAME_ACTIVE && within_goal_range(snake_head->x, snake_head->y)){ //Hit goal
		LOG("GOAL SCORED IN MAIN\r\n");
		handle_event(SCORE);
	}
}
