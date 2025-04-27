#ifndef SNAKE_LIST__H_
#define SNAKE_LIST__H_

#include <stdint.h>

#define MAX_SNAKE_LENGTH 3072 //Screen width x height

typedef struct Node {
    struct Node* next;  // pointer to the next node
    uint8_t x;
    uint8_t y;
} Node;

Node * add_to_snake_head(uint8_t x, uint8_t y);

void remove_from_snake_tail(uint8_t * x_rmv, uint8_t * y_rmv);


Node * get_snake_head();

Node * get_snake_tail();


void print_snake_list();

void delete_snake_list();

int length_of_snake_list();


#endif
