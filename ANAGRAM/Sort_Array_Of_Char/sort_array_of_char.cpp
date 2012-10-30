/*
 * sort_array_of_char.cpp
 *
 *  Created on: 20-Oct-2012
 *      Author: ankitgupta
 */

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <new>
#include <string>

using namespace std;

#define ASCII_RANGE 255

void counting_sort(char *a, char *b, int k, int l)
{
	int *c;
	int i, j;
	c = new int[k];

	for (i = 0; i < k; i++)
		c[i] = 0;

	for (j = 1; j <= l; j++)
		c[int(a[j] - ' ')]++;

	for (i = 1; i < k; i++)
		c[i] = c[i] + c[i - 1];

	for (j = l; j > 0; j--) {
		b[c[int(a[j] - ' ')]] = a[j];
		c[int(a[j] - ' ')]--;
	}
}

void is_anagram(char *a, char *b)
{
	/*
	 * Assuming strings to be NULL terminated.
	 */

	int i = 0;
	int j = 0;
	char *s_a;
	char *s_b;
	char *u_a;
	char *u_b;

	cout<<"Checking if anagram or not..."<<endl;
	while (a[i])
		i++;

	while (b[j])
		j++;

	if (i != j) {
		cout<<"\nNot an ANAGRAM\n";
		return;
	}

	s_a = new char[i + 1];
	s_b = new char[i + 1];
	u_a = new char[i + 1];
	u_b = new char[i + 1];

	u_a[0] = ' ';
	u_b[0] = ' ';

	for (int k = 1; k < (i + 2); k++)
		u_a[k] = a[k - 1];

	for (int k = 1; k < (i + 2); k++)
		u_b[k] = b[k - 1];

	s_a[0] = ' ';
	s_b[0] = ' ';

	counting_sort(u_a, s_a, ASCII_RANGE, i + 1);
	counting_sort(u_b, s_b, ASCII_RANGE, i + 1);

	for (int k = 1; k < (i + 1); k++) {
		if (s_a[k] != s_b[k]) {
			cout<<"\nNot an ANAGRAM\n";
			return;
		}
	}
	cout<<"\nStrings form an ANAGRAM\n";
}

int main()
{
	/*
	char input1[] = "afa     esfd36239463264^*^^(^^#%@aasgfag273273cslgnsvnsdv jfjadfff    sjdvsdjvjsjF";
	int i, k;

	char *input, *output;

	i = 0;
	while (input1[i])
		i++;

	input = new char[i + 1];

	input[0] = ' ';
	cout<<"\nSize is : "<<i<<endl;
	for (k = 1; k < (i + 2); k++)
		input[k] = input1[k - 1];

	output = new char[i + 1];

	output[0] = ' ';
	cout<<"\nInput array is :\n";
	cout<<input;
	cout<<endl;

	counting_sort(input, output, ASCII_RANGE, i + 1);

	cout<<"\nOutput array is :\n";
	cout<<output;
	cout<<endl;
	*/

	char a[] = "HA RRY D";
	char b[] = "RRYAD  H";

	is_anagram(a, b);

	return 0;
}
