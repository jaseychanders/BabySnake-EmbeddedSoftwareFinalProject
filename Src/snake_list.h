/**
 ******************************************************************************
 * @file           : snake_list.h
 * @author         : Jasey Chanders
 * @brief          : A singly linked list variation that stores the snake
 * @date           : April 30th, 2025
 ******************************************************************************
 */

#ifndef SNAKE_LIST__H_
#define SNAKE_LIST__H_

#include <stdint.h>

#define MAX_SNAKE_LENGTH 3072 //Screen width x height

/*
 * Singly linked list Node object
 */
typedef struct Node {
    struct Node* next;
    uint8_t x;
    uint8_t y;
} Node;

/*
 * @Breif   	: Adds a node to the head of the snake
 *
 * @Node    	: The head of the snake is the tail of the list
 *
 * @param x 	: x location of the new node
 * @param y     : y location of the new node
 *
 * @return      : The new node
 */
Node * add_to_snake_head(uint8_t x, uint8_t y);

/*
 * @Breif       : Removes the last node in the tail of the snake
 *
 * @Node        : The tail of the snake is the head of the list
 *
 * @param x_rmv : an empty variable to hold the x value of the remove node
 * @param y_rmv : an empty variable to hold the y value of the remove node
 */
void remove_from_snake_tail(uint8_t * x_rmv, uint8_t * y_rmv);

/*
 * @Breif       : Returns the head of the snake
 *
 * @Node        : The head of the snake is the tail of the list
 *
 * @return 		: The node at the head of the snake
 */
Node * get_snake_head();

/*
 * @Breif       : Returns the tail of the snake
 *
 * @Node        : The tail of the snake is the head of the list
 *
 * @return 		: The node at the tail of the snake
 */
Node * get_snake_tail();


/*
 * @Breif       : Prints the x,y coordinates of each node in the snake
 */
void print_snake_list();

/*
 * @Breif       : Frees up all memory associates with the snake
 */
void delete_snake_list();

/*
 * @Breif       : Returns the length of the snake
 *
 * @return 		: The length of the snake
 */
int length_of_snake_list();


#endif
