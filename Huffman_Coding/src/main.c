/*
 * main.c
 *
 *  Created on: Jul 4, 2013
 *      Author: Ankit Gupta
 */

#include "../include/huffman.h"
#include <stdbool.h>
#include <string.h>

int main(int argc, char **argv)
{
    charData_t      *dataTable;
    heap_t          *huffmanHeap;
    huffmanNode_t   *huffmanTree;
    huffmanCode_t   *huffmanTable;
    char            *fileName;
    char            *dumpHuffman;
    char            *dumpCharData;
    errCode_t       result;
    U32             itr;
    bool            dumpCharFreq;
    bool            dumpHuffmanCode;
    bool            verbose;
    FILE            *fp;

    huffmanHeap = NULL;
    huffmanTree = NULL;
    huffmanTable = NULL;
    dataTable = NULL;
    result = errOk;
    dumpCharFreq = false;
    dumpHuffmanCode = false;
    verbose = false;

    if (argc < 2) {
        printf("Enter Huffman_Coding.exe --help for usage\n");
        goto terminate;
    } else if (!strcmp(argv[1], "-h") || !strcmp(argv[1], "--help")
        || !strcmp(argv[1], "--h") || !strcmp(argv[1], "-help")) {
        printf("\n**************************************************\n");
        printf("            Huffman Coding Software                 \n");
        printf("**************************************************\n\n");
        printf("USAGE: Huffman_Coding.exe --fileName <filename> [OPTIONS]\n\n");
        printf("OPTIONS: \n\n");
        printf("    --help      : displays usage information\n");
        printf("    --filename  : give path to .txt file only\n");
        printf("    --dump      : enter 'charFreq' to dump character " \
            "frequency\n");
        printf("                : enter 'codeTable' to dump huffman " \
            "code table\n");
        printf("    --dumpfile  : enter path for output data to get dump " \
            "(should be a .txt file)\n");
        printf("    --verbose   : displays huffman and char data table. " \
            "\nThis option should be used at the end of commandline.");
        goto terminate;
    }

    for (itr = 1; itr < argc; itr += 2) {
        if (!strcmp(argv[itr], "--filename")) {
            fileName = argv[itr + 1];
            fp = fopen(fileName, "r");

            if (!fp) {
                ERR("Invalid file name!");
                goto terminate;
            }

            fclose(fp);
        } else if (!strcmp(argv[itr], "--dump")) {

            if (!strcmp(argv[itr + 1], "charFreq"))
                dumpCharFreq = true;
            else if (!strcmp(argv[itr + 1], "codeTable"))
                dumpHuffmanCode = true;
            else {
                ERR("Invalid choice to dump data");
                goto terminate;
            }

        } else if (!strcmp(argv[itr], "--dumpfile")) {

            if (dumpCharFreq) {
                dumpCharData = argv[itr + 1];
                fp = fopen(dumpCharData, "w");
            } else if (dumpHuffmanCode) {
                dumpHuffman = argv[itr + 1];
                fp = fopen(dumpHuffman, "w");
            } else {
                ERR("Invalid choice to dump data");
                goto terminate;
            }

            if (!fp) {
                ERR("Invalid file name!");
                goto terminate;
            }

            fclose(fp);
        } else if (!strcmp(argv[itr], "--verbose"))
            verbose = true;

    }

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
    evaluateCompression(dataTable, huffmanTable);

    if (verbose) {
        printCharDataTable(dataTable);
        printHuffmanTable(huffmanTable);
    }

    if (dumpCharFreq)
        dumpCharDataTable(dataTable, dumpCharData);

    if (dumpHuffmanCode)
        dumpHuffmanTable(huffmanTable, dumpHuffman);

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
