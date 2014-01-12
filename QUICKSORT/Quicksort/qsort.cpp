/*

 * qsort.cpp
 *
 *  Created on: 01-Oct-2012
 *      Author: ankitgupta
 */

#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace std;

#define ARRAY_SIZE 20

#define SWAP(a, b) {int c = a; \
                        a = b; \
                        b = c;}

typedef struct {
    int *data;
    int size;
} qsort_t;

void assign_mem(qsort_t **q)
{
    if (*q) {
        cout<<"\nMemory already assigned.\n";
    } else {
        (*q) = (qsort_t *)malloc(sizeof(qsort_t));
        (*q)->data = (int *)malloc(ARRAY_SIZE * sizeof(int));
        (*q)->size = ARRAY_SIZE - 1;
        cout<<"\nArray of size : "<<ARRAY_SIZE<<" created\n";
    }
}

void fill_rand_data(qsort_t *q)
{
    int i;
    int *data;
    if (q) {
         data = q->data;
         for(i = 0; i <= q->size; i++) {
            srand(time(NULL) + i + (i * i));
            data[i] = rand()%100 + 1;
         }
    }
}

void show_data(qsort_t *q)
{
    int i;
    int *data;
    if (q) {
         data = q->data;
         cout<<"\nData is:\n";
         for(i = 0; i <= q->size; i++) {
            cout<<data[i]<<" ";
         }
         cout<<"\n";
    }
}

void free_mem(qsort_t **q)
{
    if (!(*q)) {
        cout<<"\nMemory already freed up\n";
    } else {
        free((*q)->data);
        *q = NULL;
        cout<<"\nFreed up memory\n";
    }
}

int partition(qsort_t *q, int p, int r)
{
    int right_most, pivot, j;
    int *data;
    if (q) {
        data = q->data;
        right_most = data[r];
        pivot = p - 1;
        for (j = p; j < r; j++) {
            if (data[j] <= right_most) {
                pivot++;
                SWAP(data[pivot], data[j]);
            }
        }
        SWAP(data[pivot + 1], data[r]);

        return pivot + 1;
    } else {
        return -1;
    }
}

int randomized_partition(qsort_t *q, int p, int r)
{
    int i;
    int *data = q->data;
    srand(time(NULL) + 100 + p + r);
    i = p + (rand() % (r));
    SWAP(data[i], data[r]);
    return partition(q, p, r);
}


int select_k_th(qsort_t *q, int p, int r, int k)
{
    int pivot, z;
    if (p == r)
        return q->data[p];

    pivot = partition(q, p, r);
    z = pivot - p + 1;

    if (k == z)
        return q->data[pivot];
    else if (k < z)
        return select_k_th(q, p, pivot - 1, k);
    else
        return select_k_th(q, pivot + 1, r, k - z);
}

void quicksort(qsort_t *q, int p, int r)
{
    int pivot;
    if (q) {
        if (p < r) {
            pivot = partition(q, p, r);
            quicksort(q, p, pivot - 1);
            quicksort(q, pivot + 1, r);
        }
    }
}

void randomized_quicksort(qsort_t *q, int p, int r)
{
    int pivot;
    if (q) {
        if (p < r) {
            pivot = randomized_partition(q, p, r);
            randomized_quicksort(q, p, pivot - 1);
            randomized_quicksort(q, pivot + 1, r);
        }
    }
}

void quicksort_iterative(qsort_t *q, int p, int r)
{
    // Create an auxiliary stack
    int stack[r - p + 1];
    int top = -1;
    int pivot;

    // push initial values of l and h to stack
    stack[++top] = p;
    stack[++top] = r;

    // Keep popping from stack while is not empty
    while (top > -1) {
        r = stack[top--];
        p = stack[top--];

        pivot = partition(q, p, r);

        if ((pivot - 1) > p) {
            stack[++top] = p;
            stack[++top] = pivot - 1;
        }

        if ((pivot + 1) < r) {
            stack[++top] = pivot + 1;
            stack[++top] = r;
        }
    }
}

int main()
{
    qsort_t *q = NULL;
    assign_mem(&q);
    fill_rand_data(q);
    show_data(q);
    cout<<"\n 8th element is :"<<select_k_th(q, 0, q->size, 8);
    quicksort(q, 0, q->size);
    show_data(q);
    free_mem(&q);
    return 0;
}
