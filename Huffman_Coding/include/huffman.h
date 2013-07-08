/*
 * huffman.h
 *
 *  Created on: Jun 25, 2013
 *      Author: Ankit Gupta
 */

#ifndef HUFFMAN_H_
#define HUFFMAN_H_

#include "../include/heap.h"

struct charData {
    U64     index;
    char    symbol;
};

typedef struct charData charData_t;

struct huffmanNode {
    charData_t          nodeData;
    struct huffmanNode  *left;
    struct huffmanNode  *right;
};

struct huffmanCode {
    char    symbol;
    char    *code;
};

enum huffmanMoves {
    up = -1,
    still,
    left,
    right
};

typedef struct huffmanNode  huffmanNode_t;
typedef struct huffmanCode  huffmanCode_t;
typedef enum huffmanMoves   huffmanMoves_t;

errCode_t       charDataTableInit       (charData_t **dataTable);
void            charDataTableDeInit     (charData_t **dataTable);
errCode_t       populateCharDataTable   (charData_t *datatable, char *filename);
void            printCharDataTable      (charData_t *dataTable);
heap_t*         charDataBuildMinHeap    (charData_t *dataTable);
errCode_t       createHuffmanNode       (huffmanNode_t **huffmanNode);
huffmanNode_t*  createHuffmanForest     (heap_t *heap);
errCode_t       initHuffmanCodeTable    (huffmanCode_t **huffmanCodeTable);
void            createHuffmanTable      (huffmanCode_t *huffmanCode,
                                        huffmanNode_t *huffmanForest);
void            printHuffmanTable       (huffmanCode_t *huffmanCode);
void            printHuffmanForestLeaf  (huffmanNode_t *huffmanForest);
void            evaluateCompression     (charData_t *datatable,
                                        huffmanCode_t *huffmanCode);
void            freeCharDataHeap        (heap_t *heap);
void            freeHuffmanForest       (huffmanNode_t **huffmanForest);
void            freeHuffmanTable        (huffmanCode_t **huffmanCode);

#endif /* HUFFMAN_H_ */
