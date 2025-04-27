#include "snake_list_tests.h"
#include "snake_list.h"
#include "stdlib.h"
#include "log.h"

typedef int (*TestFunction)();



int test_one_cycle(){
	int success = 1;
	Node * snake_head = NULL;
    uint8_t prev_x = 0;
    uint8_t prev_y = 0;
	snake_head = add_to_snake_head(0,0);
	snake_head = add_to_snake_head(1, 1);
	remove_from_snake_tail(&prev_x, &prev_y);
	remove_from_snake_tail(&prev_x, &prev_y);

	if(get_snake_tail() != NULL){
		success = 0;
	}
	free(snake_head);
	delete_snake_list();
	return success;
}

int test_restart_list(){
	int success = 1;
    Node * snake_head = NULL;
    uint8_t prev_x = 0;
    uint8_t prev_y = 0;
	snake_head = add_to_snake_head(0,0);
	snake_head = add_to_snake_head(1, 1);
	remove_from_snake_tail(&prev_x, &prev_y);
	remove_from_snake_tail(&prev_x, &prev_y);
	snake_head = add_to_snake_head(4, 4);

	if(get_snake_tail()->x != 4){
		success = 0;
	}

	if(get_snake_head()->x != 4){
		success = 0;
	}

	free(snake_head);
	delete_snake_list();
	return success;
}

int test_length(){
	int success = 1;
	Node * snake_head = NULL;
    uint8_t prev_x = 0;
    uint8_t prev_y = 0;
	snake_head = add_to_snake_head(0,0);
	snake_head = add_to_snake_head(1, 1);

	if(length_of_snake_list() != 2){
		success = 0;
	}

	remove_from_snake_tail(&prev_x, &prev_y);
	remove_from_snake_tail(&prev_x, &prev_y);

	if(length_of_snake_list() != 0){
		success = 0;
	}

	free(snake_head);
	delete_snake_list();
	return success;
}

int test_max_length(){
	int success = 1;
	Node * snake_head = NULL;
	for (int i = 0; i < MAX_SNAKE_LENGTH + 5; i++){
		snake_head = add_to_snake_head(0,0);
	}

	if(length_of_snake_list() > MAX_SNAKE_LENGTH){
		success = 0;
	}

	free(snake_head);
	delete_snake_list();
	return success;
}

int test_min_length(){
	int success = 1;
	Node * snake_head = NULL;
	uint8_t prev_x = 0;
	uint8_t prev_y = 0;
	for (int i = 0; i < 5; i++){
		snake_head = add_to_snake_head(0,0);
	}

	if(length_of_snake_list() > MAX_SNAKE_LENGTH){
		success = 0;
	}

	for (int i = 0; i < 7; i++){
		remove_from_snake_tail(&prev_x, &prev_y);
	}

	if(length_of_snake_list() < 0){
		success = 0;
	}

	free(snake_head);
	delete_snake_list();
	return success;
}

struct {
    TestFunction function;
    const char *name;
} tests[] = {
     { test_one_cycle, "test_one_cycle" },
    { test_restart_list, "test_restart_list" },
    { test_length, "test_length" },
    { test_max_length, "test_max_length" },
    { test_min_length, "test_min_length" },
};

int test_snake_list(){

	 LOG("** Testing Snake List ** \r\n");

	int succeeded_tests = 0;
    int total_tests = sizeof(tests) / sizeof(tests[0]);

	// Loop through the tests credit ChatGPT
	for (size_t i = 0; i < total_tests; i++) {
		if (tests[i].function()) {
			succeeded_tests++;
		} else {
			LOG("%s failed\r\n", tests[i].name);
		}
	}

	LOG("Number of tests passed %i/%i\r\n", succeeded_tests, total_tests);

	if (total_tests - succeeded_tests > 0) {
		LOG("*** FAILED TESTS *** \r\n");
	}

	return succeeded_tests;
}
