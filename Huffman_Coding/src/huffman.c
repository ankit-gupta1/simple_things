#include "../include/huffman.h"

errCode_t charDataTableInit (charData_t **dataTable)
{
    U32         itr;
    errCode_t   result;

    result = errOk;
    LOG("Initialize character data table");
    *dataTable = (charData_t *)malloc(HEAP_SIZE * sizeof(charData_t));

    if (*dataTable) {
        for (itr = 0; itr < HEAP_SIZE; itr++) {
            (*dataTable)[itr].index = 0;
            (*dataTable)[itr].symbol = (char)(itr);
        }
    } else {
        result = errNoMem;
        ERR("Memory not available %d", result);
    }

    return result;
}

errCode_t initHuffmanCodeTable (huffmanCode_t **huffmanCodeTable)
{
    U32         itr;
    errCode_t   result;

    result = errOk;
    LOG("Initialize huffman code table");
    *huffmanCodeTable = (huffmanCode_t *)malloc(HEAP_SIZE * \
        sizeof(huffmanCode_t));

    if (*huffmanCodeTable) {
        for (itr = 0; itr < HEAP_SIZE; itr++) {
            (*huffmanCodeTable)[itr].symbol = (char)(itr);
            (*huffmanCodeTable)[itr].code = NULL;
        }
    } else {
        result = errInvalid;
        ERR("Huffman table not created %d", result);
    }

    return result;
}

void charDataTableDeInit (charData_t **dataTable)
{
    LOG("Freeing up character data table memory");
    free(*dataTable);
    *dataTable = NULL;
}

errCode_t populateCharDataTable (charData_t *dataTable, char *filename)
{
    FILE        *fp;
    int         temp;
    errCode_t   result;

    result = errOk;
    LOG("Populating character data table");
    fp = fopen(filename, "r");

    if (!fp) {
        result = errNoExt;
        ERR("Unable to open input file %d", result);
        goto cleanup;
    }

    temp = fgetc(fp);

    while (temp != EOF) {
        dataTable[temp].index++;
        temp = fgetc(fp);
    }

cleanup:
    fclose(fp);
    return result;
}

heap_t* charDataBuildMinHeap (charData_t *dataTable)
{
    heap_t      *heap;
    U32         itr;
    errCode_t   result;

    heap = NULL;
    result = errOk;
    LOG("Build heap of character data table");
    result = createHeap(&heap);

    if (!result) {
        for (itr = 0; itr <= heap->length; itr++) {
            heap->dataInfo[itr].data = &dataTable[itr];
            heap->dataInfo[itr].index = dataTable[itr].index;
            heap->dataInfo[itr].type = charData;
        }
    } else {
        ERR("Unable to build char data table %d", result);
        goto cleanup;
    }

    result = buildMinHeap(heap);

    if (result)
        ERR("Unable to build heap %d", result);

cleanup:
    return heap;
}

void freeCharDataHeap (heap_t *heap)
{
    LOG("Freeing character data heap");
    deleteHeap(&heap);
}

errCode_t createHuffmanNode (huffmanNode_t **huffmanNode)
{
    errCode_t   result;

    result = errOk;
    *huffmanNode = (huffmanNode_t *)malloc(sizeof(huffmanNode_t));

    if (result) {
        result = errNoMem;
        ERR("No memory available %d", result);
        goto cleanup;
    }

    (*huffmanNode)->nodeData.index = 0;
    (*huffmanNode)->left = NULL;
    (*huffmanNode)->right = NULL;

cleanup:
    return result;
}

huffmanNode_t* createHuffmanForest (heap_t *heap)
{
    huffmanNode_t   *root;
    huffmanNode_t   *left;
    huffmanNode_t   *right;
    huffmanNode_t   *tempHuffmanNode;
    heapData_t      *tempHeapData;
    charData_t      *tempCharData;
    errCode_t       result;

    result = errOk;
    root = NULL;
    left = NULL;
    right = NULL;
    tempHuffmanNode = NULL;
    tempHeapData = NULL;
    tempCharData = NULL;

    while (heap->length >= 0) {
        result = createHuffmanNode(&root);

        if (result) {
            ERR("No memory available to create huffman Node %d", result);
            goto cleanup;
        }

        tempHeapData = extractMin(heap);

        if (tempHeapData->type == charData) {
            result = createHuffmanNode(&left);

            if (result) {
                ERR("No memory available to create huffman Node %d", result);
                goto cleanup;
            }

            tempCharData = (charData_t *)(tempHeapData->data);
            left->nodeData.index = tempCharData->index;
            left->nodeData.symbol = tempCharData->symbol;
        } else if (tempHeapData->type == huffmanData) {
            tempHuffmanNode = (huffmanNode_t *)(tempHeapData->data);
            left = tempHuffmanNode;
        }

        root->left = left;
        root->nodeData.index += left->nodeData.index;

        if (heap->length >= 0) {
            tempHeapData = extractMin(heap);

            if (tempHeapData->type == charData) {
                result = createHuffmanNode(&right);

                if (result) {
                    ERR("No memory available to create huffman Node %d",
                        result);
                    goto cleanup;
                }

                tempCharData = (charData_t *)(tempHeapData->data);
                right->nodeData.index = tempCharData->index;
                right->nodeData.symbol = tempCharData->symbol;
            } else if (tempHeapData->type == huffmanData) {
                tempHuffmanNode = (huffmanNode_t *)(tempHeapData->data);
                right = tempHuffmanNode;
            }

            root->right = right;
            root->nodeData.index += right->nodeData.index;
        } else
            break;

        tempHuffmanNode = root;
        tempHeapData->index = tempHuffmanNode->nodeData.index;
        tempHeapData->data = tempHuffmanNode;
        tempHeapData->type = huffmanData;
        insertData(heap, tempHeapData);
    }

cleanup:
    return root;
}

void freeHuffmanForest (huffmanNode_t **root)
{
    if (*root) {
        freeHuffmanForest(&(*root)->left);
        freeHuffmanForest(&(*root)->right);

        if ((!(*root)->left) && (!(*root)->right))
            free(*root);

    }
}

void printHuffmanForestLeaf (huffmanNode_t *root)
{
    if (root) {
        printHuffmanForestLeaf(root->left);

        if ((!root->left) && (!root->right)) {
            LOG("Symbol %c index %llu", root->nodeData.symbol,
                root->nodeData.index);
        }

        printHuffmanForestLeaf(root->right);
    }
}

void populateHuffmanCode (huffmanCode_t *table, huffmanNode_t *root,
    U32 count, char *code, huffmanMoves_t move)
{
    U32 itr;
    U32 pos;

    if (move == left)
        code[count++] = '0';
    else if (move == right)
        code[count++] = '1';
    else if (move == up)
        count--;

    if (root) {
        move = left;
        populateHuffmanCode(table, root->left, count, code, move);

        if ((!root->left) && (!root->right)) {
            pos = ((U32)root->nodeData.symbol);
            table[pos].code = (char *)malloc(count * sizeof(char));

            for (itr = 1; itr < count; itr++)
                table[pos].code[itr - 1] = code[itr];

            table[pos].code[count - 1] = '\0';
        }

        move = right;
        populateHuffmanCode(table, root->right, count, code, move);
    } else
        move = up;
}

void createHuffmanTable (huffmanCode_t *huffmanCode,
    huffmanNode_t *huffmanForest)
{
    static U32  count;
    static char code[HEAP_SIZE];
    static      huffmanMoves_t move;

    count = 0;
    move = still;
    populateHuffmanCode(huffmanCode, huffmanForest, count, code, move);
}

void freeHuffmanTable (huffmanCode_t **huffmanCode)
{
    U32 itr;

    LOG("Freeing up Huffman Table memory");
    for (itr = 0; itr < HEAP_SIZE; itr++)
        free((*huffmanCode)[itr].code);

    free(*huffmanCode);
}

void printHuffmanTable (huffmanCode_t *huffmanCode)
{
    U32 itr;

    LOG("Printing Huffman Table");
    for (itr = 0; itr < HEAP_SIZE; itr++)
        LOG("Symbol %c  Code %s", huffmanCode[itr].symbol,
            huffmanCode[itr].code);

}

void printCharDataTable (charData_t *dataTable)
{
    U32 itr;

    LOG("Printing CharData Table");
    for (itr = 0; itr < HEAP_SIZE; itr++)
        LOG("Symbol %c  index %llu", dataTable[itr].symbol,
            dataTable[itr].index);
}

void evaluateCompression (charData_t *dataTable, huffmanCode_t *huffmanCode)
{
    U64     totalChar;
    U64     huffmanCompSpace;
    U32     itr;
    double  compressionRatio;

    totalChar = 0;
    huffmanCompSpace = 0;
    compressionRatio = 0.0;

    for (itr = 0; itr < HEAP_SIZE; itr++) {
        totalChar += dataTable[itr].index;
        huffmanCompSpace += (dataTable[itr].index * \
            (sizeof(huffmanCode[itr].code) / \
                (sizeof(huffmanCode[itr].code[0]))) >> 3);
    }

    compressionRatio = ((double)totalChar) / ((double)huffmanCompSpace);
    LOG("Compression Ratio %g", compressionRatio);
}
