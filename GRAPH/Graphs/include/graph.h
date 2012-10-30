/*
 * graph.h
 *
 *  Created on: 23-Oct-2012
 *      Author: ankitgupta
 */

#ifndef GRAPH_H_
#define GRAPH_H_

#define NO_OF_VERTICES 10

typedef enum {
	WHITE,
	GRAY,
	BLACK
} color_t;

struct vertex_t {
	int				val;
	vertex_t		*next;
};

typedef vertex_t vq_t;

typedef struct {
	vq_t *front; /*bottom*/
	vq_t *back; /*top*/
} queue_t;

typedef struct {
	color_t			color;
	unsigned int	distance;
	int				pred_index;
} vparams_t;

typedef struct {
	int				*bfs_trav;
	queue_t			*bfs_q;
} bfs_params_t;

struct list_t {
	vertex_t		*head;
	vertex_t		*tail;
	unsigned int	neighbours;
};

struct adj_list_t {
	list_t			*graph;
	vparams_t		*params;
	bfs_params_t	*bfs_params;
	unsigned int	cnctd_cmpnts;
	unsigned int	size;
	adj_list_t		*sub_graph;
};

void populate_matrix_undirected		(bool m[][NO_OF_VERTICES], unsigned int size);
void show_matrix 					(bool m[][NO_OF_VERTICES], unsigned int size);
void init_matrix					(bool m[][NO_OF_VERTICES], unsigned int size);
void init_graph						(bool m[][NO_OF_VERTICES], unsigned int size, adj_list_t **g);
void show_graph						(adj_list_t *g);

void breadth_first_search			(adj_list_t *g);
void bfs_traversal					(adj_list_t *g);
void print_bfs_paths				(adj_list_t *g);

#endif /* GRAPH_H_ */
