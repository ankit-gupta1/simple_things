/*
 * queue.h
 *
 *  Created on: 24-Oct-2012
 *      Author: ankitgupta
 */

#ifndef QUEUE_H_
#define QUEUE_H_

#include "graph.h"

void create_queue(queue_t **queue);
void free_queue(queue_t **queue);
void enqueue(queue_t *queue, int index);
int dequeue(queue_t *queue);
void show_queue(queue_t *queue);
int queue_front(queue_t *queue);


#endif /* QUEUE_H_ */
