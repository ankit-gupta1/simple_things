/*
 * main.cpp
 *
 *  Created on: 13-Oct-2012
 *      Author: ankitgupta
 */

#include <iostream>
#include <cstdlib>
#include "../include/avl.h"

using namespace std;

extern int test_array[NODE_COUNT];

int main()
{
	avl_tree_t *tree;
	tree = NULL;
	create_avl_tree(&tree);
	build_random_tree(tree, NODE_COUNT);
	show_tree_sorted(tree);

	for (int i = 0; i < NODE_COUNT; i++) {
		cout<<test_array[i]<<" ";
	}

	cout<<"\nTree height is "<<tree->height<<"\n";
	for (int i = 0; i < NODE_COUNT; i++) {
		avl_delete(tree, tree_search_iterative(tree->root, test_array[i]));
		show_tree_sorted(tree);
		cout<<"\nTree height is "<<tree->height<<"\n";
	}

	return 0;
}
