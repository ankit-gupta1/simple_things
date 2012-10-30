/*
 * heap.h
 *
 *  Created on: 01-Oct-2012
 *      Author: ankitgupta
 */

#ifndef HEAP_H_
#define HEAP_H_

#include <iostream>

#define PARENT(i)	(i >> 1)
#define LEFT(i)		((i << 1) + 1)
#define RIGHT(i)	((i << 1) + 2)

#define HEAP_SIZE 	10

#define SWAP(a, b) 	{a = a ^ b; \
                    b = a ^ b; \
                    a = a ^ b;}

typedef struct {
    int *data;
    int length;
    int size;
} heap_t;

/* Heap utility functions. */

void create_heap				(heap_t **heap);
void fill_rand_data				(heap_t *heap);
void show_heap					(heap_t *heap);
void delete_heap				(heap_t **heap);
void max_heapify				(heap_t *heap, int i);
void max_heapify_iterative		(heap_t *heap, int i);
void min_heapify				(heap_t *heap, int i);
void build_max_heap				(heap_t *heap);
void build_max_heap_iterative	(heap_t *heap);
void build_min_heap				(heap_t *heap);
void heapsort_min_max			(heap_t *heap);
void heapsort_max_min			(heap_t *heap);

#endif /* HEAP_H_ */
