/*
 * main.cpp
 *
 *  Created on: 23-Oct-2012
 *      Author: ankitgupta
 */

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <new>
#include <iomanip>
#include "../include/graph.h"

using namespace std;

int main()
{
	bool adj_matrix_u[NO_OF_VERTICES][NO_OF_VERTICES];
	bool adj_matrix_d[NO_OF_VERTICES][NO_OF_VERTICES];
	unsigned int weight_matrix_u[NO_OF_VERTICES][NO_OF_VERTICES];
	unsigned int weight_matrix_d[NO_OF_VERTICES][NO_OF_VERTICES];
	adj_list_t *graph_u; /*Undirected graph*/
	adj_list_t *graph_d; /*Directed graph*/
	adj_list_t *graph_mst_u = NULL;

	/*
	 * Operations on Undirected Graph.
	 */

	init_matrix(adj_matrix_u, NO_OF_VERTICES);
	populate_matrix_undirected(adj_matrix_u, NO_OF_VERTICES);
	populate_weight(adj_matrix_u, weight_matrix_u, NO_OF_VERTICES, false);
	show_matrix(adj_matrix_u, NO_OF_VERTICES);

	init_graph(adj_matrix_u, weight_matrix_u, NO_OF_VERTICES, &graph_u);
	show_graph(graph_u);

	bfs_traversal(graph_u);
	dfs_traversal(graph_u);
	show_graph_with_weight(graph_u);
	diameter(graph_u);
	graph_mst_u = mst_kruskal(graph_u);
	if (graph_mst_u) {
		show_graph_with_weight(graph_mst_u);
		bfs_traversal(graph_mst_u);
		diameter(graph_mst_u);
	} else {
		cout<<"\n MST not possible\n";
	}

	/*
	 * Operations on Directed Graph.
	 */

	init_matrix(adj_matrix_d, NO_OF_VERTICES);
	populate_matrix_directed(adj_matrix_d, NO_OF_VERTICES);
	populate_weight(adj_matrix_d, weight_matrix_d, NO_OF_VERTICES, true);
	show_matrix(adj_matrix_d, NO_OF_VERTICES);

	init_graph(adj_matrix_d, weight_matrix_d, NO_OF_VERTICES, &graph_d);
	show_graph(graph_d);

	bfs_traversal(graph_d);
	dfs_traversal(graph_d);
	diameter(graph_d);

	graph_analyzer(graph_d);

	return 0;
}
