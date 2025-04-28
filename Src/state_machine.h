#ifndef STATE_MACHINE__H_
#define STATE_MACHINE__H_


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

#endif
