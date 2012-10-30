/*
 * heap.cpp
 *
 *  Created on: 01-Oct-2012
 *      Author: Ankit Gupta
 */

#include <iostream>
#include <cstdlib>
#include "../include/heap.h"

using namespace std;

int main()
{
    heap_t *heap = NULL;
    create_heap(&heap);
    fill_rand_data(heap);
    show_heap(heap);
    system("pause");
    build_max_heap_iterative(heap);
    heapsort_min_max(heap);
    show_heap(heap);
    heapsort_max_min(heap);
    show_heap(heap);
    delete_heap(&heap);
    system("pause");
    return 0;
}
