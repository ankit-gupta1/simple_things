/*
 * main.cpp
 *
 *  Created on: 23-Oct-2012
 *      Author: ankitgupta
 */

#include <iostream>
#include "../include/graph.h"

using namespace std;

int main()
{
	bool adj_matrix[NO_OF_VERTICES][NO_OF_VERTICES];
	adj_list_t *graph;

	init_matrix(adj_matrix, NO_OF_VERTICES);
	populate_matrix_undirected(adj_matrix, NO_OF_VERTICES);
	show_matrix(adj_matrix, NO_OF_VERTICES);

	init_graph(adj_matrix, NO_OF_VERTICES, &graph);
	show_graph(graph);

	bfs_traversal(graph);
	dfs_traversal(graph);
	return 0;
}



