#include "state_machine_tests.h"
#include "state_machine.h"
#include "display.h"
#include "log.h"
#include <stdint.h>


typedef int (*TestFunction)();

int test_goal_gen(){
	int success = 1;
	uint8_t x;
	uint8_t y;

	for(int i = 0; i < 500; i++){
		generate_goal(&x, &y);

		if(!(x >= 0 && x < SCREEN_HEIGHT && y >= 0 && y < SCREEN_WIDTH)){
			success = 0;
			LOG("Out of range %i %i\r\n", x,y);
		}
	}

	return success;
}

int test_goal_range(){
	int success = 1;
	start_game();

	if(!within_goal_range(START_GOAL_X, START_GOAL_Y)){ //at goal
		success = 0;
	}

	if(within_goal_range(START_GOAL_X + 5, START_GOAL_Y + 5)){ // out of range
		success = 0;
	}

	if(within_goal_range(START_GOAL_X -5, START_GOAL_Y -5)){ // out of range
		success = 0;
	}

	if(within_goal_range(START_GOAL_X -5, START_GOAL_Y)){ // out of range
		success = 0;
	}

	if(within_goal_range(START_GOAL_X, START_GOAL_Y -5)){ // out of range
		success = 0;
	}

	if(!within_goal_range(START_GOAL_X +1, START_GOAL_Y +1)){ // in range off by one
		success = 0;
	}

	if(!within_goal_range(START_GOAL_X + 1, START_GOAL_Y)){ // edge
		success = 0;
	}

	if(!within_goal_range(START_GOAL_X, START_GOAL_Y + 1)){ // edge
		success = 0;
	}

	if(!within_goal_range(START_GOAL_X -1, START_GOAL_Y)){ // edge
		success = 0;
	}

	if(!within_goal_range(START_GOAL_X, START_GOAL_Y -1)){ // edge
		success = 0;
	}

	return success;
}


struct {
    TestFunction function;
    const char *name;
} state_machine_tests[] = {
     { test_goal_gen, "test_goal_gen" },
	 { test_goal_range, "test_goal_range" },
};

int test_state_machine(){

	 LOG("** Testing State Machine ** \r\n");

	int succeeded_tests = 0;
    int total_tests = sizeof(state_machine_tests) / sizeof(state_machine_tests[0]);

	// Loop through the tests credit ChatGPT
	for (size_t i = 0; i < total_tests; i++) {
		if (state_machine_tests[i].function()) {
			succeeded_tests++;
		} else {
			LOG("%s failed\r\n", state_machine_tests[i].name);
		}
	}

	LOG("Number of tests passed %i/%i\r\n", succeeded_tests, total_tests);

	if (total_tests - succeeded_tests > 0) {
		LOG("*** FAILED TESTS *** \r\n");
	}

	return succeeded_tests;
}
