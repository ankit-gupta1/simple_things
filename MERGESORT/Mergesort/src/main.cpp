/*
 * main.cpp
 *
 *  Created on: 02-Oct-2012
 *      Author: ankitgupta
 */

#include <iostream>
#include <cstdlib>
#include <ctime>
#include "../include/list.h"
using namespace std;

#define DATA_SIZE 25

int main()
{
	list_t *list = NULL;
	int i;
	create_list(&list);
	for (i = 0; i < DATA_SIZE; i++) {
		srand(time(NULL) + i + (i * i));
		insert_node(list, (rand()%100 + 1));
	}
	show_list(list);
	list = merge_sort(list);
	show_list(list);
	return 0;
}
