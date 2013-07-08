/*
 * main.c
 *
 *  Created on: Jul 4, 2013
 *      Author: Ankit Gupta
 */

#include "../include/huffman.h"

int main()
{
    charData_t      *dataTable;
    heap_t          *huffmanHeap;
    huffmanNode_t   *huffmanTree;
    huffmanCode_t   *huffmanTable;
    char            *fileName;
    errCode_t       result;

    huffmanHeap = NULL;
    huffmanTree = NULL;
    fileName = "C:\\Tips.txt";
    result = errOk;

    result = charDataTableInit(&dataTable);
    if (result) {
        ERR("Initialization of Character Data Table Failed %d", result);
        goto terminate;
    }

    result = populateCharDataTable(dataTable, fileName);
    if (result) {
        ERR("Populating Character Data Table Failed %d", result);
        goto terminate;
    }

    huffmanHeap = charDataBuildMinHeap(dataTable);
    if (!huffmanHeap) {
        ERR("Building min heap of character data table failed");
        goto terminate;
    }

    huffmanTree = createHuffmanForest(huffmanHeap);
    if (!huffmanTree) {
        ERR("Unable to create huffman Tree");
        goto terminate;
    }

    result = initHuffmanCodeTable(&huffmanTable);
    if (result) {
        ERR("Initialization of Huffman Code Table Failed %d", result);
        goto terminate;
    }

    createHuffmanTable(huffmanTable, huffmanTree);
    printHuffmanTable(huffmanTable);
    evaluateCompression(dataTable, huffmanTable);

terminate:

    if (dataTable)
        charDataTableDeInit(&dataTable);

    if (huffmanHeap)
        freeCharDataHeap(huffmanHeap);

    if (huffmanTree)
        freeHuffmanForest(&huffmanTree);

    if (huffmanTable)
        freeHuffmanTable(&huffmanTable);

    return 0;
}
