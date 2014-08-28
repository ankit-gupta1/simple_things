#include "../include/huffman.h"

/*******************************************************************************
 * Function     :   charDataTableInit
 * Synopsis     :   This function assigns memory to the character data table. It
 *                  also initializes the counter of each symbol to zero.
 * Arguments    :   @dataTable  :   Pointer to uninitialized data table.
 * Returns      :   Returns error code value.
 ******************************************************************************/

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

/*******************************************************************************
 * Function     :   initHuffmanCodeTable
 * Synopsis     :   This function initializes the table containing huffman codes
 *                  for each symbol. It assigns memory to the huffman code
 *                  table.
 * Arguments    :   @huffmanCodeTable   :   Un-initialized pointer to huffman
 *                                          code table.
 * Return       :   Returns error code value.
 ******************************************************************************/

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

/*******************************************************************************
 * Function     :   charDataTableDeInit
 * Synopsis     :   Free's up the memory allocated to character data table.
 * Arguments    :   dataTable   :   Populated data table pointer.
 * Return       :   N. A.
 ******************************************************************************/

void charDataTableDeInit (charData_t **dataTable)
{
    LOG("Freeing up character data table memory");
    free(*dataTable);
    *dataTable = NULL;
}

/*******************************************************************************
 * Function     :   populateCharDataTable
 * Synopsis     :   Populates the character data table. Reads each character of
 *                  given file and increments the counter of that particular
 *                  symbol. That way it records the frequency of characters in
 *                  a given file.
 * Arguments    :   @dataTable  :   Pointer to character data table.
 *                  @fileName   :   Filename of subject file, for whom huffman
 *                                  codes are needed to be derived.
 * Return       :   Returns error code value.
 ******************************************************************************/

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

/*******************************************************************************
 * Function     :   charDataBuildMinHeap
 * Synopsis     :   Builds the Min Heap for character data table as per their
 *                  respective frequencies. Build Heap is efficient for this
 *                  purpose as we will always be querying for data with minimum
 *                  frequency or frequency sum. Therefore, we are first creating
 *                  a Min Heap followed by populating it with character data
 *                  table frequency. Then on applying build heap operation, our
 *                  priority queue data structure is ready.
 * Arguments    :   @dataTable  :   Pointer to a populated data table.
 * Return       :   Returns the pointer to heap created. If unable to create the
 *                  heap, returns NULL.
 ******************************************************************************/

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

/*******************************************************************************
 * Function     :   freeCharDataHeap
 * Synopsis     :   Free's up the memory allocated to heap.
 * Arguments    :   @heap   :   Pointer to initialized heap.
 * Return       :   N. A.
 ******************************************************************************/

void freeCharDataHeap (heap_t *heap)
{
    LOG("Freeing character data heap");
    deleteHeap(&heap);
}

/*******************************************************************************
 * Function     :   createHuffmanNode
 * Synopsis     :   Initializes an individual huffman node. This huffman node is
 *                  created to be appended in a huffman forest. Assigns memory
 *                  to uninitialized pointer and populates its values to zero.
 * Arguments    :   @huffmanNode :  An uninitialized pointer to an individual
 *                                  huffman node.
 * Return       :   Returns error code value.
 ******************************************************************************/

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

/*******************************************************************************
 * Function     :   createHuffmanForest
 * Synopsis     :   The most quintessential function. It populates and creates
 *                  huffman tree which will be used later to extract the huffman
 *                  codes. This function works like this. It extracts the two
 *                  minimum frequency nodes from min heap, create a root node
 *                  and assigns these two minimum values as left and right child
 *                  of its root node. It also adds the frequency of its child
 *                  and stores it as its own frequency It then pushes the
 *                  resultant root node into the heap. It keeps on doing this
 *                  till the min heap gets exhausted. This results into creation
 *                  of a huffman forest. This forest or tree is important both
 *                  in code extraction as huffman compressed data decoding.
 * Arguments    :   @heap   :   Pointer to min heap.
 * Return       :   Returns the root node of huffman forest. Upon failure,
 *                  returns NULL.
 ******************************************************************************/

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

/*******************************************************************************
 * Function     :   freeHuffmanForest
 * Synopsis     :   Root of the huffman forest.
 * Arguments    :   @root   :   Root of the huffman forest.
 * Return       :   N. A.
 ******************************************************************************/

void freeHuffmanForest (huffmanNode_t **root)
{
    if (*root) {
        freeHuffmanForest(&(*root)->left);
        freeHuffmanForest(&(*root)->right);

        if ((!(*root)->left) && (!(*root)->right))
            free(*root);

    }
}

/*******************************************************************************
 * Function     :   printHuffmanForestLeaf
 * Synopsis     :   Prints the leaf nodes of huffman forests. Usually helpful
 *                  for debugging purposes as leaf nodes will always point to an
 *                  individual character and its frequency content.
 * Arguments    :   @root   :   Root of the huffman forest.
 * Return       :   N. A.
 ******************************************************************************/

void printHuffmanForestLeaf (huffmanNode_t *root)
{
    if (root) {
        printHuffmanForestLeaf(root->left);

        if ((!root->left) && (!root->right)) {
            LOG("Symbol %c index %lu", root->nodeData.symbol,
                (long unsigned)(root->nodeData.index));
        }

        printHuffmanForestLeaf(root->right);
    }
}

/*******************************************************************************
 * Function     :   populateHuffmanCode
 * Synopsis     :   Populates Huffman Code table. This is basically a recursive
 *                  function as for reading huffman code for any symbol. Its
 *                  huffman code will be a series of left ('0') and right ('1')
 *                  moves. Two get huffman code of any symbol, start from root,
 *                  keep track of your movements while reaching for a particular
 *                  symbol in the leaf node. Here, for generating huffman code
 *                  of all the symbols, we are doing an inorder tree traversal.
 *                  Whenever, function hits the leaf node, we are storing the
 *                  required moves to reach there as its huffman code.
 * Arguments    :   @root   :   Root of the huffman forest.
 *                  @table  :   Pointer to the huffman code table.
 *                  @count  :   Current level of depth in the tree.
 *                  @code   :   It keep tracks of the current moves taken to
 *                              reach at the current point
 *                  @move   :   Next move to be made while traversing the tree.
 * Return       :   N. A.
 ******************************************************************************/

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

/*******************************************************************************
 * Function     :   createHuffmanTable
 * Synopsis     :   Wrapper function to call the recursive function.
 * Arguments    :   @huffmanForest  :   Root of the huffman forest.
 *                  @huffmanCode    :   Pointer to huffman code table.
 * Return       :   N. A.
 ******************************************************************************/

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

/*******************************************************************************
 * Function     :   freeHuffmanTable
 * Synopsis     :   Freeing up the memory allocated to huffman code table
 * Arguments    :   @huffmanCode    :   Pointer to huffman code table.
 * Return       :   N. A.
 ******************************************************************************/

void freeHuffmanTable (huffmanCode_t **huffmanCode)
{
    U32 itr;

    LOG("Freeing up Huffman Table memory");
    for (itr = 0; itr < HEAP_SIZE; itr++)
        free((*huffmanCode)[itr].code);

    free(*huffmanCode);
}

/*******************************************************************************
 * Function     :   printHuffmanTable
 * Synopsis     :   Function to print huffman code table.
 * Arguments    :   @huffmanCode    :   Pointer to huffman code table.
 * Return       :   N. A.
 ******************************************************************************/

void printHuffmanTable (huffmanCode_t *huffmanCode)
{
    U32 itr;

    LOG("Printing Huffman Table");
    for (itr = 0; itr < HEAP_SIZE; itr++)
        LOG("Symbol %c  Code %s", huffmanCode[itr].symbol,
            huffmanCode[itr].code);

}

/*******************************************************************************
 * Function     :   dumpHuffmanTable
 * Synopsis     :   Function to dump huffman code table.
 * Arguments    :   @huffmanCode    :   Pointer to huffman code table.
 *                  @fileName       :   Pointer to dump file name.
 * Return       :   N. A.
 ******************************************************************************/

void dumpHuffmanTable (huffmanCode_t *huffmanCode, char *fileName)
{
    U32 itr;
    FILE *fp;

    fp = fopen(fileName, "w");

    LOG("Dumping Huffman Table");
    for (itr = 32; itr < HEAP_SIZE - 2; itr++)
        fprintf(fp, "Symbol %c  Code %s \n", huffmanCode[itr].symbol,
            huffmanCode[itr].code);

    fclose(fp);
}

/*******************************************************************************
 * Function     :   printCharDataTable
 * Synopsis     :   Function to print character data table.
 * Arguments    :   @dataTable  :   Pointer to character data table.
 * Return       :   N. A.
 ******************************************************************************/

void printCharDataTable (charData_t *dataTable)
{
    U32 itr;

    LOG("Printing CharData Table");
    for (itr = 0; itr < HEAP_SIZE; itr++)
        LOG("Symbol %c  index %lu", dataTable[itr].symbol,
            (long unsigned)(dataTable[itr].index));
}

/*******************************************************************************
 * Function     :   dumpCharDataTable
 * Synopsis     :   Function to dump character data table.
 * Arguments    :   @dataTable  :   Pointer to character data table.
 *                  @fileName   :   Pointer to dump file name.
 * Return       :   N. A.
 ******************************************************************************/

void dumpCharDataTable (charData_t *dataTable, char *fileName)
{
    U32 itr;
    FILE *fp;

    fp = fopen(fileName, "w");

    LOG("Dumping CharData Table");
    for (itr = 32; itr < HEAP_SIZE - 2; itr++)
        fprintf(fp, "Symbol %c  index %lu \n", dataTable[itr].symbol,
        		(long unsigned)(dataTable[itr].index));

    fclose(fp);
}

/*******************************************************************************
 * Function     :   evaluateCompression
 * Synopsis     :   Function to evaluate the level of achievable huffman
 *                  compression.
 * Arguments    :   @dataTable      :   Pointer to character data table.
 *                  @huffmanCode    :   Pointer to huffman code.
 * Return       :   N. A.
 ******************************************************************************/

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
