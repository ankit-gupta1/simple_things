/*
 * main.c
 *
 *  Created on: Feb 6, 2014
 *      Author: gankit
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TEST
#undef TEST

#ifndef TEST
#define NO_OF_MATRICES  15
#else
#define NO_OF_MATRICES  3
#endif

#define INF             ~(1 << 31)
#define ZERO            0

inline int min(int a, int b) {
    return (a < b) ? a : b;
}

int optimizeParenthisization (int *nXm, int **solution, int grp, int pos)
{
    int temp, size;

    if (solution[grp - 1][pos - 1] < INF)
        temp = solution[grp - 1][pos - 1];
    else {
        temp = INF;
        for (size = 1; size <= (grp - 1); size++)
        {
            temp = min(temp, (nXm[pos - 1] * nXm[size + pos - 1] * \
                nXm[grp + pos -1]) + \
                optimizeParenthisization(nXm, solution, size, pos) + \
                optimizeParenthisization(nXm, solution, grp - size, \
                size + pos));
        }
    }

    solution[grp - 1][pos - 1] = temp;

    return temp;
}

void optimizeParenthisizationBottomUp (int *nXm, int **solution, int grp)
{
    int temp, size, pos, loc;

    for (pos = 1; pos <= grp; pos++)
    {
        solution[1 - 1][pos - 1] = 0;
    }

    for (pos = 1; pos <= (grp - 1); pos++)
    {
        solution[2 - 1][pos - 1] = nXm[pos - 1] * nXm[pos] * nXm[pos + 1];
    }

    for (size = 3; size <= grp; size++)
    {
        for (pos = 1; pos <= (grp - size + 1); pos++)
        {
            temp = INF;
            for (loc = 1; loc <= (size - 1); loc++) {
                temp = min(temp, (nXm[pos - 1] * nXm[loc + pos - 1] * \
                    nXm[size + pos - 1]) + \
                    solution[loc - 1][pos - 1] + \
                    solution[size - loc - 1][loc + pos - 1]);
            }
            solution[size - 1][pos - 1] = temp;
        }
    }
}

void optimizeParenthisizationWrapper (int *nXm, int **solution, int grp)
{
    int size, pos;

    for (pos = 1; pos <= grp; pos++)
        solution[1 - 1][pos - 1] = 0;

    for (pos = 1; pos <= (grp - 1); pos++)
        solution[2 - 1][pos - 1] = nXm[pos - 1] * nXm[pos] * nXm[pos + 1];

    for (size = 3; size <= grp; size++)
        for (pos = 1; pos <= (grp - size + 1); pos++)
                solution[size - 1][pos - 1] = INF;

    pos = optimizeParenthisization(nXm, solution, grp, 1);
}

void displaySolutionMatrix (int **solution, int grp)
{
    int size, pos;

    for (size = 1; size <= grp; size++) {
        for (pos = 1; pos <= (grp - size + 1); pos++) {
            printf(" %06d,", solution[size - 1][pos - 1]);
        }
        printf("\n\n");
    }
}

void displayMatrixChain (int *n_X_m, int grp)
{
    int size;

    for (size = 1; size <= grp; size++) {
        printf(" %03d X %03d", n_X_m[size - 1], n_X_m[size]);
        printf("\n\n");
    }
}

int main()
{
    int *nXm, size, order;
    int **solution;
    order = NO_OF_MATRICES;
    nXm = (int *)malloc((order + 1) * sizeof(int));

    solution = (int **)malloc(order * sizeof(int *));
    for (size = 1; size <= order; size++)
    {
        solution[size - 1] = (int *)malloc((order - size + 1) * sizeof(int));
    }

#ifndef TEST
    for (size = 1; size <= (order + 1); size++)
    {
        srand(time(NULL) + size * size + 100);
        nXm[size - 1] = 1 + rand() % 99;
    }
#else
     /* Test sequence, set NO_OF_MATRICES = 3 and then test.
      * Answer must be 7500.*/
     nXm[0] = 10;
     nXm[1] = 100;
     nXm[2] = 5;
     nXm[3] = 50;
#endif

    displayMatrixChain(nXm, order);
    optimizeParenthisizationWrapper(nXm, solution, order);
    displaySolutionMatrix(solution, order);
    optimizeParenthisizationBottomUp(nXm, solution, order);
    displaySolutionMatrix(solution, order);

    return 0;
}
