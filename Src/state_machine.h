#ifndef STATE_MACHINE__H_
#define STATE_MACHINE__H_

#include <stdint.h>

#define START_GOAL_X 27
#define START_GOAL_Y 15

typedef enum {
	GAME_START,
	GAME_ACTIVE,
	GAME_ENDED,
	GOAL_SCORED
} State;

typedef enum {
    START,
    FAIL,
    SCORE,
	SCORE_COMP
} Event;


typedef struct {
    State current_state;
    Event event;
    State next_state;
    void (*action)(void);  // function pointer for state transition
} Transition;


int start_game();

void update_sate_machine();

void handle_event(Event event);

//Testing util functions
void generate_goal(uint8_t *x, uint8_t *y);
int within_goal_range(uint8_t x, uint8_t y);

#endif
