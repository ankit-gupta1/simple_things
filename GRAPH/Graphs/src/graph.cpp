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
#include <iomanip>
#include "../include/graph.h"
#include "../include/queue.h"

using namespace std;

static unsigned int time_count;
static unsigned int elem_count;

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

void populate_matrix_directed(bool M[][NO_OF_VERTICES], unsigned int size)
{
	unsigned int i, j;

	for (i = 0; i < size; i++) {
		for (j = 0; j < size; j++) {
			srand(time(NULL) + i * j * 7 + (1900 *j));
			if (!M[i][j])
				M[i][j] = (rand() % 2) ? true : false;
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

void init_dfs_params(dfs_params_t **dfs_params, int size)
{
	(*dfs_params)->dfs_trav = new int[size];
	(*dfs_params)->dfs_q = NULL;
	create_queue(&(*dfs_params)->dfs_q);
}

void init_graph(bool M[][NO_OF_VERTICES], unsigned int size, adj_list_t **g)
{
	unsigned int i, j;
	vertex_t *v = NULL;

	*g			= (adj_list_t *)malloc(sizeof(adj_list_t));
	(*g)->graph		= (list_t *)malloc(size * (sizeof(list_t)));
	(*g)->bfs_vparams	= (bfs_vparams_t *)malloc(size * sizeof(bfs_vparams_t));
	(*g)->bfs_params	= (bfs_params_t *)malloc(sizeof(bfs_params_t));
	(*g)->dfs_vparams	= (dfs_vparams_t *)malloc(size * sizeof(dfs_vparams_t));
	(*g)->dfs_params	= (dfs_params_t *)malloc(sizeof(dfs_params_t));
	(*g)->sub_graph		= (adj_list_t *)malloc(sizeof(adj_list_t));

	init_bfs_params(&(*g)->bfs_params, size);
	init_dfs_params(&(*g)->dfs_params, size);

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
	g->cnctd_cmpnts = 0;

	/*
	 * Randomly pick any initial point in a graph.
	 */

	srand(time(NULL) +1001);
	s = rand() % g->size;

	for (i = 0; i < g->size; i++) {
		g->bfs_vparams[i].color = WHITE;
		g->bfs_vparams[i].distance = (~(1 << 31));
		g->bfs_vparams[i].pred_index = 1 << 31;
	}

	g->bfs_vparams[s].color = GRAY;
	g->bfs_vparams[s].distance = 0;
	g->bfs_vparams[s].pred_index = 1 << 31;

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
				if (g->bfs_vparams[v].color == WHITE) {
					g->bfs_vparams[v].color = GRAY;
					g->bfs_vparams[v].distance = g->bfs_vparams[u].distance + 1;
					g->bfs_vparams[v].pred_index = u;
					enqueue(q, v);
				}

				v_v = v_v->next;
			}
			g->bfs_vparams[u].color = BLACK;
		} else {
			for (j = 0; j < g->size; j++)
				if (g->bfs_vparams[j].color == WHITE)
					break;

			g->bfs_vparams[j].color = GRAY;
			g->bfs_vparams[j].distance = 0;
			g->bfs_vparams[j].pred_index = 1 << 31;

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
					cout<<setw(2)<<trav[k]<<" ";

				cout<<"\n Distance    : ";
				for (k = j; k < s; k++)
					cout<<setw(2)<<g->bfs_vparams[trav[k]].distance<<" ";

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

void bfs_parameterized(adj_list_t *g, unsigned int init_point)
{
	unsigned int s, i, j;
	int u, v;
	queue_t *q = NULL;
	queue_t *qt = NULL;
	int *trav = NULL;
	vertex_t *v_v = NULL;
	g->cnctd_cmpnts = 0;

	/*
	 * Randomly pick any initial point in a graph.
	 */

	s = init_point;

	for (i = 0; i < g->size; i++) {
		g->bfs_vparams[i].color = WHITE;
		g->bfs_vparams[i].distance = (~(1 << 31));
		g->bfs_vparams[i].pred_index = 1 << 31;
	}

	g->bfs_vparams[s].color = GRAY;
	g->bfs_vparams[s].distance = 0;
	g->bfs_vparams[s].pred_index = 1 << 31;

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
				if (g->bfs_vparams[v].color == WHITE) {
					g->bfs_vparams[v].color = GRAY;
					g->bfs_vparams[v].distance = g->bfs_vparams[u].distance + 1;
					g->bfs_vparams[v].pred_index = u;
					enqueue(q, v);
				}

				v_v = v_v->next;
			}
			g->bfs_vparams[u].color = BLACK;
		} else {
			for (j = 0; j < g->size; j++)
				if (g->bfs_vparams[j].color == WHITE)
					break;

			g->bfs_vparams[j].color = GRAY;
			g->bfs_vparams[j].distance = 0;
			g->bfs_vparams[j].pred_index = 1 << 31;

			enqueue(q, j);
			enqueue(qt, i);
			g->cnctd_cmpnts++;
		}
	}

	enqueue(qt, g->size);

	qt = NULL;
	free_queue(&q);
}

void dfs_visit(adj_list_t *g, unsigned int u)
{
	unsigned int v;
	vertex_t *v_u = NULL;

	g->dfs_vparams[u].color = GRAY;
	time_count++;
	g->dfs_vparams[u].discovery_time = time_count;

	v_u = g->graph[u].head;

	while (v_u) {
		v = v_u->val;
		if (g->dfs_vparams[v].color == WHITE) {
			g->dfs_vparams[v].pred_index = u;
			dfs_visit(g, v);
		}

		v_u = v_u->next;
	}

	time_count++;
	g->dfs_vparams[u].color = BLACK;
	g->dfs_vparams[u].finish_time = time_count;
	g->dfs_params->dfs_trav[elem_count++] = u;
}

void depth_first_search(adj_list_t *g)
{
	queue_t *q = NULL;
	unsigned int i;

	g->cnctd_cmpnts = 0;
	q = g->dfs_params->dfs_q;

	for (i = 0; i < g->size; i++) {
		g->dfs_vparams[i].color = WHITE;
		g->dfs_vparams[i].pred_index = 1 << 31;
	}

	time_count = 0;
	elem_count = 0;

	for (i = 0; i < g->size; i++) {
		if (g->dfs_vparams[i].color == WHITE) {
			enqueue(q, i);
			g->cnctd_cmpnts++;
			dfs_visit(g, i);
		}
	}

	enqueue(q, g->size);
	time_count = 0;
	elem_count = 0;
}

void print_dfs_paths(adj_list_t *g)
{
	unsigned int i, j, k, s;
	queue_t *q = NULL;
	int *trav = NULL;

	q = g->dfs_params->dfs_q;
	trav = g->dfs_params->dfs_trav;

	s = dequeue(q);
	if (g) {
		if (queue_front(q) > -1) {
			j = 0;

			for (i = 1; i <= g->cnctd_cmpnts; i++) {
				cout<<"\n\nGraph component "<<i<<endl;

				cout<<"\n DFS search order  :\n";
				cout<<"\n Visit order       : ";

				s = dequeue(q);

				for (k = j; k < s; k++)
					cout<<setw(2)<<trav[k]<<" ";

				cout<<"\n Discovery Time    : ";
				for (k = j; k < s; k++)
					cout<<setw(2)<<g->dfs_vparams[trav[k]].discovery_time<<" ";

				cout<<endl;

				cout<<"\n Finishing Time    : ";
				for (k = j; k < s; k++)
					cout<<setw(2)<<g->dfs_vparams[trav[k]].finish_time<<" ";

				cout<<endl;
				j = s;
			}
		}
	}
}

void dfs_traversal(adj_list_t *g)
{
	if (g) {
		depth_first_search(g);
		print_dfs_paths(g);
	}
}

void diameter(adj_list_t *g)
{
	unsigned int i = 0;
	unsigned int init_point;
	unsigned int end_point;
	unsigned int max_distance = 0;
	unsigned int temp;

	if (!g)
		return;

	breadth_first_search(g);
	if (g->cnctd_cmpnts > 0)
		return;

	for (i = 0; i < g->size; i++) {
		bfs_parameterized(g, i);
		temp = g->bfs_vparams[g->bfs_params->bfs_trav[g->size - 1]].distance;
		if (temp > max_distance) {
			max_distance = temp;
			init_point = i;
			end_point = g->bfs_params->bfs_trav[g->size - 1];
		}
	}

	cout<<"\nDiameter of Graph is : "<<max_distance<<endl;
	cout<<"\nInitial Point        : "<<init_point<<endl;
	cout<<"\nEnd Point            : "<<end_point<<endl;
}
