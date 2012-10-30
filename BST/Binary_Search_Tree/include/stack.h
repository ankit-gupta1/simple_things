/*
 * stack.h
 *
 *  Created on: 06-Oct-2012
 *      Author: ankitgupta
 */

#ifndef STACK_H_
#define STACK_H_

#include "bst.h"

typedef struct {
	node_stack_t *head; /*bottom*/
	node_stack_t *tail; /*top*/
} stack_t;

void create_stack(stack_t **stack);
void free_stack(stack_t **stack);
void copy_stack(stack_t **a, stack_t **b);
void push(stack_t *stack, node_t *node);
node_t *pop(stack_t *stack);
void show_stack(stack_t *stack);
node_t *top(stack_t *stack);

#endif /* STACK_H_ */
