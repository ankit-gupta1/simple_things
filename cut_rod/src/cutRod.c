/*
 * cutRod.c
 *
 *  Created on: Jan 12, 2014
 *      Author: gankit
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define LENGTH      10          // Size of rod in any unit.
#define BASE_PRICE  10          // Constant base price of rod.
#define BASE_MIN    7           /* Constant to offer variation in rods pricing
                                 * successive lengths. */
#define MINUS_INF   (1 << 31)   // Constant for minus infinity.

inline int max(int a, int b) {
    return (a > b) ? a : b;
}

/**
 * This structure is for storing solutions of rod cutting problem. Field left
 * indicates the longest portion of the rod cut and right field denotes the
 * other sub-part. One most explore right sub-parts to explore further parts
 * of breaking rods.
 */
typedef struct breakUp {
    int left;
    int right;
} breakUp_t;

/**
 * memoizedCutRodAux:   Main recursive routine.
 * @ params:    price:      Pointer to array of prices of rods of different size
 *                          sold without cutting.
 *              revenue:    Maximum revenue generated by selling rods of each
 *                          individual size.
 *              size:       Size of the rod for which maximum revenue generated
 *                          needs to be determined.
 * @ return:    This function returns the revenue for each of the size.
 */
int memoizedCutRodAux (int *price, int *revenue, int size)
{
    int temp = 0;
    int i = 0;

    /* First check if the maximum revenue problem for given size of rod is
     * solved or not. If not, then its revenue field must have a value smaller
     * than 0. Otherwise, return the maximum revenue for this size. Note that
     * here we have saved potential repetitive calls by verifying if solution to
     * given problem has already been solved.*/
    if (size > 0)
        if (revenue[size - 1] >= 0)
            return revenue[size - 1];

    /* If size is zero, then you will get zero value when you sell it.*/
    if (size == 0)
        temp = 0;
    else {  /* Otherwise determine the best solution for a rod of given size.*/
        /*Now check the maximum revenue for each possibility.*/
        for (i = 1; i <= size; i++)
            temp = max(temp, price[i - 1] + memoizedCutRodAux(price, revenue,
                    size - i));
    }

    /* Do not populate revenue for temp values smaller than 0 as size might be
     * already zero, might cause buffer corruption. here we are storing solution
     * of each size for future references in other stack calls.*/
    if (temp > 0)
        revenue[size - 1] = temp;

    return temp;
}

/**
 * memoizedCutRodAuxSol:    Main recursive routine. This routine will also store
 *                          solution for each size.
 * @ params:    price:      Pointer to array of prices of rods of different size
 *                          sold without cutting.
 *              revenue:    Maximum revenue generated by selling rods of each
 *                          individual size.
 *              solution:   Pointer to array of breakup struct for storing
 *                          solution of each individual sizes.
 *              size:       Size of the rod for which maximum revenue generated
 *                          needs to be determined.
 * @ return:    This function returns the revenue for each of the size.
 */
int memoizedCutRodAuxSol (int *price, int *revenue, breakUp_t *solution, int size)
{
    int temp = 0;
    int i = 0;
    int prev = 0;

    /* First check if the maximum revenue problem for given size of rod is
     * solved or not. If not, then its revenue field must have a value smaller
     * than 0. Otherwise, return the maximum revenue for this size. Note that
     * here we have saved potential repetitive calls by verifying if solution to
     * given problem has already been solved.*/
    if (size > 0)
        if (revenue[size - 1] >= 0)
            return revenue[size - 1];

    /* If size is zero, then you will get zero value when you sell it.*/
    if (size == 0)
        temp = 0;
    else {  /* Otherwise determine the best solution for a rod of given size.*/
        /*Now check the maximum revenue for each possibility.*/
        temp = MINUS_INF;
        for (i = 1; i <= size; i++) {
            prev = temp;
            /* Here recursively call the same routine to determine the revenue
             * from sub-part.*/
            temp = max(temp, price[i - 1] + memoizedCutRodAuxSol(price, revenue,
                    solution,
                    size - i));
            /* Store the solution for maximum revenue case.*/
            if (temp > prev) {
                solution[size - 1].left = i;
                solution[size - 1].right = size - i;
            }
        }
    }

    /* Do not populate revenue for temp values smaller than 0 as size might be
     * already zero, might cause buffer corruption. here we are storing solution
     * of each size for future references in other stack calls.*/
    if (temp > 0)
        revenue[size - 1] = temp;

    return temp;
}

/**
 * memoizedCutRod:  Wrapper function to top down memoized solution of cut rod
 *                  problem. Initially the whole revenue array is set
 *                  to smallest integer value.
 * @params: price:      Pointer to array of prices of rods of different size
 *                      sold without cutting.
 *          revenue:    Maximum revenue generated by selling rods of each
 *                      individual size.
 *          size:       Size of the rod for which maximum revenue generated
 *                      needs to be determined.
 */
void memoizedCutRod (int *price, int *revenue, int size)
{
    int i = 0;

    /* Initializing input arrays.*/
    for (i = 0; i < size; i++)
        revenue[i] = MINUS_INF;

    /* Calling main cut rod sub routine.*/
    i = memoizedCutRodAux(price, revenue, size);
}

/**
 * memoizedCutRodSol:   Wrapper function to top down memoized solution of cut
 *                      rod problem. Initially the whole revenue array is set
 *                      to smallest integer value. And all solutions are
 *                      initialized as if selling the whole rod without
 *                      cutting is profitable.
 * @params: price:      Pointer to array of prices of rods of different size
 *                      sold without cutting.
 *          revenue:    Maximum revenue generated by selling rods of each
 *                      individual size.
 *          solution:   Pointer to array of breakup struct for storing
 *                      solution of each individual sizes.
 *          size:       Size of the rod for which maximum revenue generated
 *                      needs to be determined.
 */
void memoizedCutRodSol (int *price, int *revenue, breakUp_t *solution, int size)
{
    int i = 0;

    /* Initializing input arrays.*/
    for (i = 0; i < size; i++) {
        revenue[i] = MINUS_INF;
        solution[i].left = i + 1;
        solution[i].right = 0;
    }

    /* Calling main recursive function.*/
    i = memoizedCutRodAuxSol(price, revenue, solution, size);
}

/**
 * bottomUpCutRod:  Bottom up approach.
 * @params: price:      Pointer to array of prices of rods of different size
 *                      sold without cutting.
 *          revenue:    Maximum revenue generated by selling rods of each
 *                      individual size.
 *          size:       Size of the rod for which maximum revenue generated
 *                      needs to be determined.
 */
void bottomUpCutRod (int *price, int *revenue, int size)
{
    int i = 0, j = 0, temp;

    /* Start to solve the problem from smallest size to original size of rod.
     * So what happens is this, for longer sizes, you don't have to
     * repetitively evaluate smaller problems.*/
    for (i = 1; i <= size; i++) {
        temp = MINUS_INF;

        /* Here we are sure that revenue for smaller sizes has already been
         * generated and hence no need to solve smaller problems.*/
        for (j = 1; j < i; j++)
            temp = max(temp, price[j - 1] + revenue[i - j - 1]);

        temp = max(temp, price[j - 1]);
        revenue[i - 1] = temp;
    }
}

/**
 * bottomUpCutRodSol:   A solution with bottom up approach. We are also storing
 *                      solutions here.
 * @params: price:      Pointer to array of prices of rods of different size
 *                      sold without cutting.
 *          revenue:    Maximum revenue generated by selling rods of each
 *                      individual size.
 *          solution:   Pointer to array of breakup struct for storing
 *                      solution of each individual sizes.
 *          size:       Size of the rod for which maximum revenue generated
 *                      needs to be determined.
 */
void bottomUpCutRodSol (int *price, int *revenue, breakUp_t *solution, int size)
{
    int i = 0, j = 0, temp = 0, prev = 0;

    /* Start to solve the problem from smallest size to original size of rod.
     * So what happens is this, for longer sizes, you don't have to
     * repetitively evaluate smaller problems.*/
    for (i = 1; i <= size; i++) {
        temp = MINUS_INF;

        /* Here we are sure that revenue for smaller sizes has already been
         * generated and hence no need to solve smaller problems.*/
        for (j = 1; j < i; j++) {
            prev = temp;
            temp = max(temp, price[j - 1] + revenue[i - j - 1]);
            if (temp > prev) {
                solution[i - 1].left = j;
                solution[i - 1].right = i - j;
            }
        }

        prev = temp;
        temp = max(temp, price[j - 1]);
        if (temp > prev) {
            solution[i - 1].left = j;
            solution[i - 1].right = 0;
        }

        revenue[i - 1] = temp;
    }
}

void displayPriceCharacteristics (int *price, int *revenue, int size)
{
    int i = 0;

    printf("length    price     revenue\n");
    for (i = 0; i < size; i++)
        printf("  %2d        %3d       %3d\n", i + 1, price[i], revenue[i]);
}

void displaySolution (breakUp_t *solution, int size)
{
    int i = 0, temp = 0;

    for (i = 1; i <= size; i++) {
        temp = i;
        while (temp > 0) {
            printf("%d ", solution[temp - 1].left);
            temp = solution[temp - 1].right;
        }

        printf("\n");
    }
}

int main()
{
    int *price;
    int *revenue;
    breakUp_t *solution;
    int size = LENGTH;
    int i = 0;

    price = (int *)malloc(size * sizeof(int));
    revenue = (int *)malloc(size * sizeof(int));
    solution = (breakUp_t *)malloc(size * sizeof(breakUp_t));

    srand(time(NULL) + i * i + 1120);
    price[0] = BASE_PRICE - (rand() % BASE_PRICE);
    for (i = 1; i < size; i++) {
        srand(time(NULL) + i * i + 100);
        price[i] = price[i - 1] + (rand() % BASE_PRICE);
        price[i] = ((price[i] * (BASE_MIN + (rand() % (13 - BASE_MIN)))) / 10);
    }

    memoizedCutRodSol(price, revenue, solution, size);
    displayPriceCharacteristics(price, revenue, size);
    displaySolution(solution, size);

    free(price);
    free(revenue);
    free(solution);
    return 0;
}
