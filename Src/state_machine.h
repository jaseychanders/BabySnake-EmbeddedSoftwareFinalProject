/**
 ******************************************************************************
 * @file           : state_machine.h
 * @author         : Jasey Chanders
 * @brief          : State machine that controls game updates and transitions
 * @date           : April 30th, 2025
 ******************************************************************************
 */

#ifndef STATE_MACHINE__H_
#define STATE_MACHINE__H_

#include <stdint.h>

#define START_GOAL_X 27
#define START_GOAL_Y 15

// Game states
typedef enum {
	GAME_START,
	GAME_ACTIVE,
	GAME_ENDED,
	GOAL_SCORED
} State;


// Game Events
typedef enum {
    START,
    FAIL,
    SCORE,
	SCORE_COMP
} Event;

//Transition table objects
typedef struct {
    State current_state;
    Event event;
    State next_state;
    void (*action)(void);  // function pointer for state transition
} Transition;

/*
 * @Brief		: Starts the game
 */
void start_game();

/*
 * @Brief		: Handle next tick of state machine
 * 				: First updates the screen with movement of the snake calculated in the previous tick
 * 				: Then checks for button presses
 * 				: Then updates the snake list
 * 				: If a goal is reached, only process the goal and do not move the snake
 */
void update_sate_machine();

/*
 * @Brief		: Process and event
 *
 * @Param event	: The event to process
 */
void handle_event(Event event);

//Testing util functions

/*
 * @Brief		: Creates a goal point
 *
 * @Param x		: Empty variable to hold the generated x value
 * @Param y		: Empty variable to hold the generated y value
 */
void generate_goal(uint8_t *x, uint8_t *y);

/*
 * @Brief		: Checks if the a close enough to the goal
 *
 * @Param x		: The x value of the point to check
 * @Param y		: The y value of the point to check
 */
int within_goal_range(uint8_t x, uint8_t y);

#endif
