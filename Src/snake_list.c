#include <stdlib.h>
#include "log.h"
#include "snake_list.h"

static Node * head;
static Node * tail;

static int length = 0;


Node* init_snake_list(uint8_t x, uint8_t y) {
	LOG("Init\r\n");
    Node* new_node = malloc(sizeof(Node));
    if (!new_node) return NULL;

    new_node->next = NULL;
    new_node->x = x;
    new_node->y = y;

    head = new_node;
    tail = new_node;
    length ++;

    return head;
}


Node * add_to_snake_head(uint8_t x, uint8_t y){
	LOG("Add Head\r\n");
	Node* new_node = malloc(sizeof(Node));
	if (!new_node) return NULL;

	new_node->next = NULL;
	new_node->x = x;
	new_node->y = y;

	head->next = new_node;
	head = new_node;
	length ++;
	return head;
}

void remove_from_snake_tail(uint8_t * x_rmv, uint8_t * y_rmv){
	LOG("RM tail\r\n");
	if(tail != NULL){
		Node* old_tail = tail;
		tail = old_tail->next;
		*x_rmv = old_tail->x;
		*y_rmv = old_tail->y;
		length --;
		free(old_tail);
	}
}

int length_of_snake_list(){
	return length;
}


void print_snake_list(){
	LOG("PRINT\r\n");
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
}
