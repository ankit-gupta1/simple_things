/*
 * stack.cpp
 *
 *  Created on: 06-Oct-2012
 *      Author: ankitgupta
 */

#include <iostream>
#include <cstdlib>
#include "../include/stack.h"

using namespace std;

void create_stack(stack_t **stack)
{
	if (!*stack) {
		*stack = (stack_t *)malloc(sizeof(stack_t));
		(*stack)->head = NULL;
		(*stack)->tail = NULL;
	}
}

void copy_stack(stack_t **a, stack_t **b)
{
	/*
	 * Copying a to b.
	 */

	node_t *new_node = NULL;
	node_stack_t *curr_node = NULL;

	curr_node = (*a)->head;
	while (curr_node) {
		new_node = (node_t *)malloc(sizeof(node_t));
		new_node = curr_node->node;
		push(*b, new_node);
		new_node = NULL;
		curr_node = curr_node->next;
	}
}

void free_stack(stack_t **stack)
{
	node_stack_t *node;
	if(*stack) {
		while ((*stack)->tail) {
			node = (*stack)->tail;
			(*stack)->tail = node->prev;
			free(node);
		}
		free(*stack);
		*stack = NULL;
	}
}

void push(stack_t *stack, node_t *node)
{
    node_stack_t *curr_node, *new_node;
    if (!stack)
       return;

    new_node = (node_stack_t *)malloc(sizeof(node_stack_t));
    new_node->node = node;
    new_node->next = NULL;
    new_node->prev = NULL;

    if (!stack->head) {
        stack->head = new_node;
        stack->tail = stack->head;
    } else {
        curr_node = stack->tail;
        curr_node->next = new_node;
        curr_node->next->prev = curr_node;
        stack->tail = curr_node->next;
    }
}

node_t *pop(stack_t *stack)
{
    node_t *node = NULL;
    node_stack_t *curr_node = NULL;
    if (!stack)
       return node;

    if (!stack->head)
       return node;

    curr_node = stack->tail;
    if (!curr_node->prev) {
    	stack->tail = NULL;
    	stack->head = NULL;
    } else {
    	curr_node->prev->next = NULL;
    	stack->tail = curr_node->prev;
    }
    return curr_node->node;
}

void show_stack(stack_t *stack)
{
	node_stack_t *curr_node = NULL;
	if (!stack)
		return;

	curr_node = stack->head;
	cout<<"\nStack growing downwards\n";
	while (curr_node) {
		cout<<curr_node->node->key<<"\n";
		curr_node = curr_node->next;
	}
}

node_t *top(stack_t *stack)
{
	node_stack_t *top_node = NULL;
	if (!stack)
		return NULL;
	else if (!stack->head)
		return NULL;
	else {
		top_node = stack->tail;
		return top_node->node;
	}
}
