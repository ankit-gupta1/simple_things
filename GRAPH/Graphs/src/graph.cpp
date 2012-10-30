/*
 * graph.cpp
 *
 *  Created on: 23-Oct-2012
 *      Author: ankitgupta
 */

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <new>
#include "../include/graph.h"
#include "../include/queue.h"

using namespace std;

void init_matrix(bool M[][NO_OF_VERTICES], unsigned int size)
{
	unsigned int i, j;

	for (i = 0; i < size; i++) {
		for (j = 0; j < size; j++) {
			M[i][j] = false;
		}
	}
}

void populate_matrix_undirected(bool M[][NO_OF_VERTICES], unsigned int size)
{
	unsigned int i, j;

	for (i = 0; i < size; i++) {
		for (j = i; j < size; j++) {
			srand(time(NULL) + i * j * 7 + 100 *j);
			M[i][j] = (rand() % 2) ? true : false;
			M[j][i] = M[i][j];
		}
	}
}

void show_matrix(bool M[][NO_OF_VERTICES], unsigned int size)
{
	unsigned int i, j;

	cout<<"\nAdjacency matrix is :\n";
	for (i = 0; i < size; i++) {
		for (j = 0; j < size; j++) {
			cout<<" "<<M[i][j];
		}
		cout<<"\n";
	}
}

void create_vertex(vertex_t **v, int val)
{
	if (!*v) {
		*v = new vertex_t;
		(*v)->val = val;
		(*v)->next = NULL;
	}
}

void create_list(list_t **list)
{
	if (!*list) {
		(*list)->head = NULL;
		(*list)->tail = NULL;
		(*list)->neighbours = 0;
 	}
}

void append_vertex(list_t *list, vertex_t *v)
{
	vertex_t *curr = NULL;

	if (!list)
		return;

	if (!v)
		return;

	if (!list->head) {
		list->head = v;
		list->tail = v;
	} else {
		curr = list->tail;
		curr->next = v;
		list->tail = v;
	}

	list->neighbours++;
}

void init_bfs_params(bfs_params_t **bfs_params, int size)
{
	(*bfs_params)->bfs_trav = new int[size];
	(*bfs_params)->bfs_q = NULL;
	create_queue(&(*bfs_params)->bfs_q);
}

void init_graph(bool M[][NO_OF_VERTICES], unsigned int size, adj_list_t **g)
{
	unsigned int i, j;
	vertex_t *v = NULL;

	*g					= (adj_list_t *)malloc(sizeof(adj_list_t));
	(*g)->graph			= (list_t *)malloc(size * (sizeof(list_t)));
	(*g)->params		= (vparams_t *)malloc(size * sizeof(vparams_t));
	(*g)->bfs_params	= (bfs_params_t *)malloc(sizeof(bfs_params_t));
	(*g)->sub_graph		= (adj_list_t *)malloc(sizeof(adj_list_t));

	init_bfs_params(&(*g)->bfs_params, size);

	for (i = 0; i < size; i++) {
		(*g)->graph[i].head = NULL;
		(*g)->graph[i].tail = NULL;
		(*g)->graph[i].neighbours = 0;
		for (j = 0; j < size; j++) {
			if (M[i][j]) {
				create_vertex(&v, j);
				append_vertex(&(*g)->graph[i], v);
				v = NULL;
			}
		}
	}

	(*g)->cnctd_cmpnts = 0;
	(*g)->size = size;
}

void show_graph(adj_list_t *g)
{
	unsigned int i;
	vertex_t *v = NULL;

	cout<<"\nAdjacency list is :\n";
	for (i = 0; i < g->size; i++) {
		cout<<"\n["<<i<<"]";
		v = g->graph[i].head;

		while (v) {
			cout<<" -> "<<v->val;
			v = v->next;
		}
		cout<<endl;
	}
}

void breadth_first_search(adj_list_t *g)
{
	unsigned int s, i, j;
	int u, v;
	queue_t *q = NULL;
	queue_t *qt = NULL;
	int *trav = NULL;
	vertex_t *v_v = NULL;

	/*
	 * Randomly pick any initial point in a graph.
	 */

	srand(time(NULL) +1001);
	s = rand() % g->size;

	for (i = 0; i < g->size; i++) {
		g->params[i].color = WHITE;
		g->params[i].distance = (~(1 << 31));
		g->params[i].pred_index = 1 << 31;
	}

	g->params[s].color = GRAY;
	g->params[s].distance = 0;
	g->params[s].pred_index = 1 << 31;

	create_queue(&q);
	enqueue(q, s);

	qt = g->bfs_params->bfs_q;
	trav = g->bfs_params->bfs_trav;
	i = 0;

	while (q->front || (i < g->size)) {
		u = dequeue(q);
		if (u > -1) {
			v_v = g->graph[u].head;
			trav[i++] = u;

			while (v_v) {
				v = v_v->val;
				if (g->params[v].color == WHITE) {
					g->params[v].color = GRAY;
					g->params[v].distance = g->params[u].distance + 1;
					g->params[v].pred_index = u;
					enqueue(q, v);
				}

				v_v = v_v->next;
			}
			g->params[u].color = BLACK;
		} else {
			for (j = 0; j < g->size; j++)
				if (g->params[j].color == WHITE)
					break;

			g->params[j].color = GRAY;
			g->params[j].distance = 0;
			g->params[j].pred_index = 1 << 31;

			enqueue(q, j);
			enqueue(qt, i);
			g->cnctd_cmpnts++;
		}
	}

	enqueue(qt, g->size);

	qt = NULL;
	free_queue(&q);
}

void print_bfs_paths(adj_list_t *g)
{
	unsigned int i, j, k, s;
	queue_t *q = NULL;
	int *trav = NULL;

	if (g) {
		q = g->bfs_params->bfs_q;
		trav = g->bfs_params->bfs_trav;
		if (queue_front(q) > -1) {
			j = 0;

			for (i = 0; i <= g->cnctd_cmpnts; i++) {
				cout<<"\n\nGraph component "<<(i + 1)<<endl;

				cout<<"\n BFS search order :\n";
				cout<<"\n Visit order : ";

				s = dequeue(q);

				for (k = j; k < s; k++)
					cout<<trav[k]<<" ";

				cout<<"\n Distance    : ";
				for (k = j; k < s; k++)
					cout<<g->params[trav[k]].distance<<" ";

				cout<<endl;
				j = s;
			}
		}
	}
}

void bfs_traversal(adj_list_t *g)
{
	if (g) {
		breadth_first_search(g);
		print_bfs_paths(g);
	}
}
