/*
 * main.cpp
 *
 *  Created on: 10-Oct-2012
 *      Author: ankitgupta
 */

#include <iostream>
#include "../include/rb_tree.h"

using namespace std;

extern int test_array[NODE_COUNT];
node_t *NIL;

int main()
{
	tree_t *tree;
	tree = NULL;
	create_rb_tree(&tree);
	build_random_tree(tree, NODE_COUNT);
	show_tree_sorted(tree);

	for (int i = 0; i < NODE_COUNT; i++) {
		cout<<test_array[i]<<" ";
	}

	rb_delete(tree, tree_search_iterative(tree->root, test_array[0]));
	rb_delete(tree, tree_search_iterative(tree->root, test_array[3]));
	rb_delete(tree, tree_search_iterative(tree->root, test_array[7]));

	show_tree_sorted(tree);
	return 0;
}




