/*
 * heap_utils.cpp
 *
 *  Created on: 01-Oct-2012
 *      Author: ankitgupta
 */

#include <cstdlib>
#include <ctime>
#include "../include/heap.h"
using namespace std;

void create_heap(heap_t **heap)
{
    if ((*heap)) {
        cout<<"\nHeap already created\n";
    } else {
        *heap = (heap_t *)malloc(sizeof(heap_t));
        (*heap)->data = (int *)malloc(HEAP_SIZE * sizeof(int));
        (*heap)->length = HEAP_SIZE - 1;
        (*heap)->size = 0;
        cout<<"\nCreated an empty heap of size "<<HEAP_SIZE<<"\n";
    }
}

void fill_rand_data(heap_t *heap)
{
    int *data;
    int i;

    if (!heap)
        return;

    data = heap->data;
    heap->size = heap->length;
    for (i = 0; i <= heap->size; i++) {
        srand(time(NULL) + i + (i * i));
        data[i] = rand()%100 + 1;
    }
}

void show_heap(heap_t *heap)
{
    int i;

    if (!heap)
        return;

    int *data = heap->data;
    cout<<"\nCurrent heap is this:\n";
    for (i = 0; i <= heap->length; i++) {
        cout<<data[i]<<" ";
    }
    cout<<"\n";
}

void delete_heap(heap_t **heap)
{
    if (!(*heap)) {
        cout<<"\nHeap already freed up\n";
    } else {
        free((*heap)->data);
        *heap = NULL;
        cout<<"\nFreed up heap\n";
    }
}

void max_heapify(heap_t *heap, int i)
{
    int l, r, largest, size;
    int *data;

    if (!heap)
        return;

    data = heap->data;
    size = heap->size;
    if (i > (size/2))
        return;

    l = LEFT(i);
    r = RIGHT(i);

    if ((l <= size) && (data[l] > data[i]))
        largest = l;
    else
        largest = i;

    if ((r <= size) && (data[r] > data[largest]))
        largest = r;

    if (largest != i) {
        SWAP(data[i], data[largest]);
        max_heapify(heap, largest);
    }
}

void max_heapify_iterative(heap_t *heap, int i)
{
    int l, r, largest, size;
    int *data;

    data = heap->data;
    size = heap->size;

    if (!heap || (i > (size >> 1)))
        return;

    while (i <= (size >> 1)) {
        l = LEFT(i);
        r = RIGHT(i);
        if ((l <= size) && (data[l] > data[i]))
            largest = l;
        else
            largest = i;

        if ((r <= size) && (data[r] > data[largest]))
            largest = r;

        if (largest != i) {
            SWAP(data[i], data[largest]);
            i = largest;
        } else {
            break;
        }
    }
}

void min_heapify(heap_t *heap, int i)
{
    int l, r, smallest, size;
    int *data;

    if (!heap)
        return;

    data = heap->data;
    size = heap->size;
    if (i > (size/2))
        return;

    l = LEFT(i);
    r = RIGHT(i);

    if ((l <= size) && (data[l] < data[i]))
        smallest = l;
    else
        smallest = i;

    if ((r <= size) && (data[r] < data[smallest]))
        smallest = r;

    if (smallest != i) {
        SWAP(data[i], data[smallest]);
        min_heapify(heap, smallest);
    }
}

void build_max_heap(heap_t *heap)
{
    int i;

    if (!heap)
        return;

    heap->size = heap->length;
    for (i = ((heap->length) >> 1); i >= 0; i--)
        max_heapify(heap, i);
}

void build_max_heap_iterative(heap_t *heap)
{
    int i;

    if (!heap)
        return;

    heap->size = heap->length;
    for (i = ((heap->length) >> 1); i >= 0; i--)
        max_heapify_iterative(heap, i);
}

void build_min_heap(heap_t *heap)
{
    int i;

    if (!heap)
        return;

    heap->size = heap->length;
    for (i = ((heap->length) >> 1); i >= 0; i--)
        min_heapify(heap, i);
}

void heapsort_min_max(heap_t *heap)
{
    int i;
    int *data;

    if (!heap)
        return;

    data = heap->data;
    build_max_heap(heap);
    for (i = heap->length; i >= 1; i--) {
        SWAP(data[0], data[i]);
        heap->size--;
        max_heapify(heap, 0);
    }
}

void heapsort_max_min(heap_t *heap)
{
    int i;
    int *data;

    if (!heap)
        return;

    data = heap->data;
    build_min_heap(heap);
    for (i = heap->length; i >= 1; i--) {
        SWAP(data[0], data[i]);
        heap->size--;
        min_heapify(heap, 0);
    }
}
