/*
 * queue.cpp
 *
 *  Created on: 07-Oct-2012
 *      Author: ankitgupta
 */

#include <iostream>
#include <cstdlib>
#include "../include/queue.h"

using namespace std;

void create_queue(queue_t **queue)
{
	if (!*queue) {
		*queue = (queue_t *)malloc(sizeof(queue_t));
		(*queue)->front = NULL;
		(*queue)->back = NULL;
	}
}

void free_queue(queue_t **queue)
{
	node_queue_t *node;
	if(*queue) {
		while ((*queue)->front) {
			node = (*queue)->front;
			(*queue)->front = node->prev;
			(*queue)->front->next = NULL;
			free(node);
		}
		free(*queue);
		*queue = NULL;
	}
}

void enqueue(queue_t *queue, node_t *node)
{
    node_queue_t *curr_node, *new_node;
    if (!queue)
       return;

    new_node = (node_queue_t *)malloc(sizeof(node_queue_t));
    new_node->node = node;
    new_node->next = NULL;
    new_node->prev = NULL;

    if (!queue->front) {
    	queue->back = new_node;
    	queue->front = queue->back;
    } else {
        curr_node = queue->back;
        curr_node->prev = new_node;
        new_node->next = curr_node;
        queue->back = new_node;
    }
}

node_t *dequeue(queue_t *queue)
{
    node_t *node = NULL;
    node_queue_t *prev_node, *curr_node;
    if (!queue)
       return node;

    if (!queue->front)
       return node;

    curr_node = queue->front;
    prev_node = curr_node->prev;
    if (!prev_node) {
    	queue->front = NULL;
    	queue->back = NULL;
    } else {
    	prev_node->next = NULL;
    	queue->front = prev_node;
    }
    return curr_node->node;
}

void show_queue(queue_t *queue)
{
	node_queue_t *curr_node = NULL;
	if (!queue)
		return;

	curr_node = queue->back;
	cout<<"\nQueue\n";
	while (curr_node) {
		cout<<curr_node->node->key<<"\n";
		curr_node = curr_node->next;
	}
}
