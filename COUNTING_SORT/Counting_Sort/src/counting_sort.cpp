/*
 * counting_sort.cpp
 *
 *  Created on: 19-Oct-2012
 *      Author: ankitgupta
 */

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <new>

using namespace std;

#define RANGE		6
#define ELEMENTS	15

/*
 * Adopted from CLR book.
 */

void counting_sort(int *a, int *b, int k, int l)
{
	int *c;
	int i, j;
	c = new int[k];

	for (i = 0; i < k; i++)
		c[i] = 0;

	for (j = 1; j <= l; j++)
		c[a[j]]++;

	for (i = 1; i < k; i++)
		c[i] = c[i] + c[i - 1];

	for (j = l; j > 0; j--) {
		b[c[a[j]]] = a[j];
		c[a[j]]--;
	}
}

int main()
{
	int *input, *output;
	int i;

	input = new int[ELEMENTS + 1];
	output = new int[ELEMENTS + 1];

	cout<<"Input array is :\n";
	for (i = 1; i <= ELEMENTS; i++) {
		srand(time(NULL) + i * i * 7 + 100);
		input[i] = rand() % RANGE;
		cout<<input[i]<<" ";
	}
	cout<<endl;

	counting_sort(input, output, RANGE, ELEMENTS);

	cout<<"Output array is :\n";
	for (i = 1; i <= ELEMENTS; i++) {
		cout<<output[i]<<" ";
	}
	cout<<endl;

	return 0;
}
