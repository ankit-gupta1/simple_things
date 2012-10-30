/*
 * queue.h
 *
 *  Created on: 07-Oct-2012
 *      Author: ankitgupta
 */

#ifndef QUEUE_H_
#define QUEUE_H_

#include "bst.h"

typedef struct {
	node_queue_t *front; /*bottom*/
	node_queue_t *back; /*top*/
} queue_t;

void create_queue(queue_t **queue);
void free_queue(queue_t **queue);
void enqueue(queue_t *queue, node_t *node);
node_t *dequeue(queue_t *queue);
void show_queue(queue_t *queue);

#endif /* QUEUE_H_ */
