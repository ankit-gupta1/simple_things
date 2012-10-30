/*
 * main.cpp
 *
 *  Created on: 05-Oct-2012
 *      Author: ankitgupta
 */
#include <iostream>
#include "../include/bst.h"

using namespace std;

extern int test_array[NODE_COUNT];

int main()
{
	tree_t *tree;
	tree = NULL;
	create_tree(&tree);
	build_random_tree(tree, NODE_COUNT);

	for (int i = 0; i < NODE_COUNT; i++) {
		cout<<test_array[i]<<" ";
	}

	cout<<"\nHeight of tree : "<<tree_height_iterative(tree)<<"\n";

	morris_traversal(tree);
	//inorder_tree_walk_iterative(tree);

	lowest_common_ancestor(tree, test_array[37], test_array[78]);

	free_tree(&tree);

	return 0;
}
