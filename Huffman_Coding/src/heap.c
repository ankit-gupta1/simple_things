/*
 * heap.c
 *
 *  Created on: Jun 23, 2013
 *      Author: Ankit Gupta
 */

#include "../include/heap.h"
#include "../include/huffman.h"

/*******************************************************************************
 * Function     :   createHeap
 * Synopsis     :   Initializes the heap data structure, assigns memory to it as
 * 					well.
 * Arguments    :   @heap	:	Double pointer to heap data structure.
 * Return       :   Returns error code value if no memory is available.
 ******************************************************************************/

errCode_t createHeap (heap_t **heap)
{
    errCode_t result;

    LOG("Creating a heap");
    result = errOk;
    *heap = (heap_t *)malloc(sizeof(heap_t));

    if (!*heap) {
        result = errNoMem;
        ERR("Memory not available %d", result);
        goto cleanup;
    }

    (*heap)->dataInfo = (heapData_t *)malloc(HEAP_SIZE * sizeof(heapData_t));

    if (!(*heap)->dataInfo) {
        result = errNoMem;
        ERR("Memory not available %d", result);
        goto cleanup;
    }

    (*heap)->length = HEAP_SIZE - 1;
    LOG("Created an empty heap of size %u", HEAP_SIZE);

cleanup:
    return result;
}

/*******************************************************************************
 * Function     :   showHeap
 * Synopsis     :   Displays the current elements queued up in heap.
 * Arguments    :   @heap	:	Pointer to heap data structure.
 * Return       :   N. A.
 ******************************************************************************/

void showHeap (heap_t *heap)
{
    int             i;
    char            symbol;
    charData_t      *tempCharData;
    huffmanNode_t   *tempHuffmanData;
    errCode_t       result;

    result = errOk;

    if (!heap) {
        result = errInvalid;
        ERR("Heap not initialized %d", result);
        return;
    }

    heapData_t *heapData = heap->dataInfo;
    LOG("Current heap is this:");

    for (i = 0; i <= heap->length; i++) {

        if (heapData[i].type == charData) {
            tempCharData = (charData_t *)heapData[i].data;
            symbol = tempCharData->symbol;
        } else if (heapData[i].type == huffmanData) {
            tempHuffmanData = (huffmanNode_t *)heapData[i].data;
            symbol = tempHuffmanData->nodeData.symbol;
        }

        LOG("Symbol %c  %llu ", symbol, heapData[i].index);
    }

    LOG("");
}

/*******************************************************************************
 * Function     :   deleteHeap
 * Synopsis     :   Frees up the memory allocated to heap data structure.
 * Arguments    :   @heap	:	Double pointer to heap data structure.
 * Return       :   N. A.
 ******************************************************************************/

void deleteHeap (heap_t **heap)
{
    if (!(*heap)) {
        ERR("Heap already freed up");
    } else {
        free((*heap)->dataInfo);
        free(*heap);
    }
}

/*******************************************************************************
 * Function     :   minHeapify
 * Synopsis     :   Does the minimum heapify operation.
 * Arguments    :   @heap	:	Double pointer to heap data structure.
 * 					@i		:	Location in the array where heapify is to be
 * 								performed.
 * Return       :   N. A.
 ******************************************************************************/

void minHeapify (heap_t *heap, int i)
{
    int l, r, smallest, length;
    heapData_t *data;

    if (!heap) {
        ERR("Heap not initialized");
        return;
    }

    data = heap->dataInfo;
    length = heap->length;
    if (i > (length/2))
        return;

    l = LEFT(i);
    r = RIGHT(i);

    if ((l <= length) && (data[l].index < data[i].index))
        smallest = l;
    else
        smallest = i;

    if ((r <= length) && (data[r].index < data[smallest].index))
        smallest = r;

    if (smallest != i) {
        SWAP(data[i], data[smallest]);
        minHeapify(heap, smallest);
    }
}

/*******************************************************************************
 * Function     :   buildMinHeap
 * Synopsis     :   Builds the min Heap.
 * Arguments    :   @heap	:	Pointer to heap data structure.
 * Return       :   Returns invalid error code when heap is not initialized.
 ******************************************************************************/

errCode_t buildMinHeap (heap_t *heap)
{
    int         itr;
    errCode_t   result;

    result = errOk;

    if (!heap) {
        result = errInvalid;
        ERR("Heap not initialized %d", result);
        goto cleanup;
    }

    LOG("Building min heap");

    for (itr = ((heap->length) >> 1); itr >= 0; itr--)
        minHeapify(heap, itr);

cleanup:
    return result;
}

/*******************************************************************************
 * Function     :   extractMin
 * Synopsis     :   Extract the topmost element in the heap.
 * Arguments    :   @heap	:	Pointer to heap data structure.
 * Return       :   Returns pointer to min extracted heap node.
 ******************************************************************************/

heapData_t* extractMin (heap_t *heap)
{
    heapData_t* heapData;
    errCode_t result;

    result = errOk;
    heapData = NULL;

    if (!heap) {
        result = errInvalid;
        ERR("Heap not initialized %d", result);
        goto cleanup;
    }

    if (heap->length == 0) {
        heapData =  &heap->dataInfo[heap->length];
        heap->length--;
        goto cleanup;
    }

    SWAP(heap->dataInfo[0], heap->dataInfo[heap->length]);
    heapData = &heap->dataInfo[heap->length];
    heap->length--;
    minHeapify(heap, 0);

cleanup:
    return heapData;
}

void insertData (heap_t *heap, heapData_t *heapData)
{
    errCode_t result;

    result = errOk;

    if (!heap) {
        result = errNoMem;
        ERR("Heap not initialized %d", result);
        return;
    }

    heap->length++;
    heap->dataInfo[heap->length].data = heapData->data;
    heap->dataInfo[heap->length].index = heapData->index;
    heap->dataInfo[heap->length].type = heapData->type;

    minHeapify(heap, heap->length >> 1);
}
