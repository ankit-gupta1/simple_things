/*
 * queue.cpp
 *
 *  Created on: 24-Oct-2012
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
	vq_t *node;
	if(*queue) {
		while ((*queue)->back) {
			node = (*queue)->back;
			(*queue)->back = node->next;
			free(node);
		}
		free(*queue);
		*queue = NULL;
	}
}

void enqueue(queue_t *queue, int index)
{
    vq_t *curr_node, *new_node;
    if (!queue)
       return;

    new_node = (vq_t *)malloc(sizeof(vq_t));
    new_node->val = index;
    new_node->next = NULL;

    if (!queue->front) {
    	queue->back = new_node;
    	queue->front = queue->back;
    } else {
        curr_node = queue->back;
        new_node->next = curr_node;
        queue->back = new_node;
    }
}

int dequeue(queue_t *queue)
{
    vq_t *curr_node;
    if (!queue)
       return -1;

    if (!queue->front)
       return -1;

    curr_node = queue->back;

    if (curr_node != queue->front) {
    	while (curr_node->next != queue->front)
    		curr_node = curr_node->next;

    	queue->front = curr_node;
    	curr_node = curr_node->next;
    } else {
    	queue->front = NULL;
    	queue->back = NULL;
    }

    return curr_node->val;
}

void show_queue(queue_t *queue)
{
	vq_t *curr_node = NULL;
	if (!queue)
		return;

	curr_node = queue->back;
	cout<<"\nQueue\n";
	while (curr_node) {
		cout<<curr_node->val<<"\n";
		curr_node = curr_node->next;
	}
}

int queue_front(queue_t *queue)
{
	if (queue) {
		if (queue->front)
			return queue->front->val;
		else
			return -1;
	} else
		return -1;
}
