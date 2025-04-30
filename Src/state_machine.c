/**
 ******************************************************************************
 * @file           : state_machine.c
 * @author         : Jasey Chanders
 * @brief          : State machine that controls game updates and transitions
 * @date           : April 30th, 2025
 ******************************************************************************
 */

#include "state_machine.h"
#include "log.h"
#include "display.h"
#include "delay.h"
#include "led.h"
#include "snake_list.h"
#include <stdlib.h>

#include "switches.h"

#define SNAKE_LENGTH_INCRESE	3

#define GOAL_RADIUS				2



uint8_t flags;			// Bit flags used to check for button presses
uint8_t prev_x;			// Temporarily stores the x value of the tail of the snake
uint8_t prev_y;			// Temporarily stores the y value of the tail of the snake
int direction;			// Direction of travel of the snake
Node * snake_head;		// Node at the head of the snake
int length_of_snake;	// Length the current snake is allowed to grow to
uint8_t goal_x; 		// Current goal x
uint8_t goal_y;			// Current goal y

static State current_state = GAME_START; //Tracks the current state


/*
 * @Brief		: Transition function to reset the game
 */
void reset_game() {
	LOG("Starting Game\r\n");
	direction = UP;
	length_of_snake = 5;
	goal_x = START_GOAL_X;
	goal_y = START_GOAL_Y;
	set_pixel(goal_x, goal_y, 1);			// Put goal pixel on screen
	snake_head = add_to_snake_head(0, 0);	// Put head of the snake on screen
	clear_flags();
}

/*
 * @Brief		: Transition function called when a goal is reached
 */
void process_goal() {
	LOG("Goal Scored\r\n");
	length_of_snake += SNAKE_LENGTH_INCRESE;// Make the snake longer
	set_pixel(goal_x, goal_y, 0);			// Remove the current goal from the screen
	generate_goal(&goal_x, &goal_y);		// Create a new gaol
	while(goal_x < 2 						// Regenerate goal if it is out of range
			|| goal_x > SCREEN_HEIGHT -2
			|| goal_y < 2
			|| goal_y > SCREEN_WIDTH -2){
		generate_goal(&goal_x, &goal_y);
	}
	LOG("New Goal %i %i\r\n", goal_x, goal_y);
	set_pixel(goal_x, goal_y, 1);			// Put new goal on the screen
	push_display();							// Update the display
	handle_event(SCORE_COMP);				// Update state machine that the goal has been processed
}

/*
 * @Brief		: Transition function called when the game is over
 */
void end_game() {
	LOG("You Died\r\n");
	delete_snake_list();					// Delete the snake
	free(snake_head);
	snake_head = NULL;
	clear_display();						// Clear the display
	delay_ms(100);
	fail_screen();							// Display fail screen
	delay_ms(1000);
	clear_display();						// Clear the display
	handle_event(START);					// Update the state machine to restart the game
}

/*
 * @Brief		: Transition function called when nothing needs to happen
 */
void do_nothing() {

}

// State transition table
// {Current State,	Event,		Next State,		Transition Function}
Transition state_table[] = {
    {GAME_START,    START,		GAME_ACTIVE, 	reset_game},
    {GAME_ACTIVE, 	SCORE,   	GOAL_SCORED, 	process_goal},
	{GOAL_SCORED, 	SCORE_COMP, GAME_ACTIVE, 	do_nothing},
    {GAME_ACTIVE,   FAIL, 		GAME_ENDED,  	end_game},
	{GAME_ENDED,   	START, 		GAME_ACTIVE, 	reset_game}
};

//See header for details
void start_game(){
	handle_event(START);
}

//See header for details
void update_sate_machine(){

	if(current_state == GAME_ACTIVE && !within_goal_range(snake_head->x, snake_head->y)){	// Game is active but a goal hasn't been hit

		if(length_of_snake_list() > length_of_snake){										// If the snake is at max length, remove the last node in the tail
			remove_from_snake_tail(&prev_x, &prev_y);
		}
		set_pixel(snake_head->x, snake_head->y, 1);											// Put the snake head added in the last update on the screen
		set_pixel(prev_x, prev_y, 0);
		push_display();

		flags = get_flags();																// Check for button presses via interrupt flags
		if(flags) {
			if (flags & UP_SWITCH_MSK && direction != DOWN){											// Up switch
				direction = UP;
			} else if (flags & RIGHT_SWITCH_MSK && direction != LEFT){									// Right switch
				direction = RIGHT;
			} else if (flags & DOWN_SWITCH_MSK && direction != UP){									// Down switch
				direction = DOWN;
			} else if (flags & LEFT_SWITCH_MSK && direction != RIGHT){									// Left switch
				direction = LEFT;
			}
			clear_flags();
		}

		switch (direction){																	// Handle adding the new snake head based on direction of travel
			case UP: {																		// Traveling Up
				if(snake_head->y < SCREEN_WIDTH -1){
					snake_head = add_to_snake_head(snake_head->x, snake_head->y + 1);
				} else {
					handle_event(FAIL);
				}
				break;
			}
			case RIGHT: {																		// Traveling Right
				if( snake_head->x < SCREEN_HEIGHT -1){
					snake_head = add_to_snake_head(snake_head->x + 1, snake_head->y);
				} else {
					handle_event(FAIL);
				}
				break;
			}
			case  DOWN: {																		// Traveling Down
				if (snake_head->y > 1){
					snake_head = add_to_snake_head(snake_head->x, snake_head->y - 1);
				} else {
					handle_event(FAIL);
				}
				break;
			}
			case LEFT: {																		// Traveling Left
				if(snake_head->x > 1){
					snake_head = add_to_snake_head(snake_head->x - 1, snake_head->y);
				}	else {
					handle_event(FAIL);
				}
				break;
			}
			default:
				LOG("No direction specified\r\n");
		}

	} else if(current_state == GAME_ACTIVE && within_goal_range(snake_head->x, snake_head->y)){ // Game is active and a goal has been hit
		LOG("GOAL SCORED IN MAIN\r\n");
		handle_event(SCORE);
	}
}

//See header for details
void handle_event(Event event){
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

//See header for details
void generate_goal(uint8_t *x, uint8_t *y){
	*x = GOAL_RADIUS + rand() % (SCREEN_HEIGHT - GOAL_RADIUS - 1);
	*y = GOAL_RADIUS + rand() % (SCREEN_WIDTH  - GOAL_RADIUS - 	1);
}

//See header for details
int within_goal_range(uint8_t x, uint8_t y){
	return (abs(x - goal_x) < GOAL_RADIUS) && (abs(y - goal_y) < GOAL_RADIUS);
}
