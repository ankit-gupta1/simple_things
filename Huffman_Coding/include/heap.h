/*
 * heap.h
 *
 *  Created on: Jun 23, 2013
 *      Author: Ankit Gupta
 */

#ifndef HEAP_H_
#define HEAP_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef __FILENAME__
#define __FILENAME__ ""
#endif

#ifndef __LINE__
#define __LINE__ ""
#endif

#define LOG(message, ...) printf("LOG @ %d of %s : " message "\n", __LINE__, \
    __FILENAME__, ##__VA_ARGS__);

#define ERR(message, ...) printf("ERROR @ %d of %s : " message "\n", __LINE__, \
    __FILENAME__, ##__VA_ARGS__);

#define PARENT(i)   (i >> 1)
#define LEFT(i)     ((i << 1) + 1)
#define RIGHT(i)    ((i << 1) + 2)

#define HEAP_SIZE   128

typedef __uint64_t U64;
typedef __uint32_t U32;

enum dataType {
    charData = 0,
    huffmanData
};

enum errCode {
    errOk       = 0,
    errNoMem    = -1,
    errNoExt    = -2,
    errInvalid  = -3,
    errNoData   = -4
};

typedef enum dataType   dataType_t;
typedef enum errCode    errCode_t;

struct heapData {
    U64         index;
    dataType_t  type;
    void        *data;
};

typedef struct heapData heapData_t;

struct heap {
    heapData_t  *dataInfo;
    int         length;
};

typedef struct heap heap_t;

#define SWAP(x,y) do \
    { \
        unsigned char swap_temp[(signed)sizeof(x)]; \
        memcpy(swap_temp,&y,sizeof(x)); \
        memcpy(&y,&x,       sizeof(x)); \
        memcpy(&x,swap_temp,sizeof(x)); \
    } while(0)

/* Heap utility functions. */

errCode_t   createHeap      (heap_t **heap);
void        showHeap        (heap_t *heap);
void        deleteHeap      (heap_t **heap);
void        minHeapify      (heap_t *heap, int i);
errCode_t   buildMinHeap    (heap_t *heap);
heapData_t* extractMin      (heap_t *heap);
void        insertData      (heap_t *heap, heapData_t *heapData);

#endif /* HEAP_H_ */
