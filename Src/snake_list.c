#include <stdlib.h>
#include "log.h"
#include "snake_list.h"

static Node * head;
static Node * tail;

static int length = 0;

Node * get_snake_head(){
	return head;
}

Node * get_snake_tail(){
	return tail;
}


Node * add_to_snake_head(uint8_t x, uint8_t y){

	if(length >= MAX_SNAKE_LENGTH){
		return NULL;
	}

	Node* new_node = malloc(sizeof(Node));
	if (!new_node) return NULL;

	new_node->next = NULL;
	new_node->x = x;
	new_node->y = y;

	if(head == NULL){
		head = new_node;
		tail = new_node;
	} else {
		head->next = new_node;
		head = new_node;
	}
	length ++;

	return head;
}

void remove_from_snake_tail(uint8_t * x_rmv, uint8_t * y_rmv){
	if(tail != NULL){

		Node* old_tail = tail;
		tail = old_tail->next;
		*x_rmv = old_tail->x;
		*y_rmv = old_tail->y;
		length --;
		free(old_tail);

		if(tail == NULL){
			head = NULL;
			length = 0;
		}
	}
}

int length_of_snake_list(){
	return length;
}


void print_snake_list(){
	Node* ptr = tail;
	while(ptr != NULL){
		LOG("x %i, y %i, ", ptr->x, ptr->y);
		ptr = ptr->next;
	}
	LOG("\r\n");
}


void delete_snake_list(){
	while(tail != NULL){
		Node* old_tail = tail;
		tail = tail->next;
		free(old_tail);
	}
	head = NULL;
	tail = NULL;
	length = 0;
}
