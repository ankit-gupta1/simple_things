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
	bool adj_matrix_u[NO_OF_VERTICES][NO_OF_VERTICES];
	bool adj_matrix_d[NO_OF_VERTICES][NO_OF_VERTICES];
	adj_list_t *graph_u; /*Undirected graph*/
	adj_list_t *graph_d; /*Directed graph*/

	/*
	 * Operations on Undirected Graph.
	 */

	init_matrix(adj_matrix_u, NO_OF_VERTICES);
	populate_matrix_undirected(adj_matrix_u, NO_OF_VERTICES);
	show_matrix(adj_matrix_u, NO_OF_VERTICES);

	init_graph(adj_matrix_u, NO_OF_VERTICES, &graph_u);
	show_graph(graph_u);

	bfs_traversal(graph_u);
	dfs_traversal(graph_u);

	/*
	 * Operations on Directed Graph.
	 */

	init_matrix(adj_matrix_d, NO_OF_VERTICES);
	populate_matrix_undirected(adj_matrix_d, NO_OF_VERTICES);
	show_matrix(adj_matrix_d, NO_OF_VERTICES);

	init_graph(adj_matrix_d, NO_OF_VERTICES, &graph_d);
	show_graph(graph_d);

	bfs_traversal(graph_d);
	dfs_traversal(graph_d);

	return 0;
}



