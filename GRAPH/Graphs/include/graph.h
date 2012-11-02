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

typedef enum {
	TREE,
	BACK,
	CROSS,
	NIL
} edge_type_t;

struct vertex_t {
	int			val;
	vertex_t		*next;
};

/*
 * Structure edge is like this.
 * 		u----(w)----v
 */

struct edge_t {
	edge_type_t		edge_type;
	unsigned int		u;
	unsigned int		v;
	unsigned int		w;
	edge_t			*next;
};

typedef vertex_t vq_t;

typedef struct {
	vq_t			*front; /*bottom*/
	vq_t			*back; /*top*/
} queue_t;

typedef struct {
	color_t			color;
	unsigned int	distance;
	int				pred_index;
} bfs_vparams_t;

typedef struct {
	int				*bfs_trav;
	queue_t			*bfs_q;
} bfs_params_t;

typedef struct {
	color_t			color;
	unsigned int	discovery_time;
	unsigned int	finish_time;
	int 			pred_index;
} dfs_vparams_t;

typedef struct {
	int				*dfs_trav;
	queue_t			*dfs_q;
} dfs_params_t;

struct list_t {
	vertex_t		*head;
	vertex_t		*tail;
	unsigned int	neighbours;
};

struct list_edge_t {
	edge_t			*head;
	edge_t			*tail;
};

struct adj_list_t {
	list_t			*graph;
	list_edge_t		*edge;
	bfs_vparams_t	*bfs_vparams;
	bfs_params_t	*bfs_params;
	dfs_vparams_t	*dfs_vparams;
	dfs_params_t	*dfs_params;
	unsigned int	cnctd_cmpnts;
	unsigned int	size;
	adj_list_t		*sub_graph;
};

void populate_matrix_undirected		(bool m[][NO_OF_VERTICES], unsigned int size);
void populate_matrix_directed		(bool M[][NO_OF_VERTICES], unsigned int size);
void show_matrix 					(bool m[][NO_OF_VERTICES], unsigned int size);
void init_matrix					(bool m[][NO_OF_VERTICES], unsigned int size);
void init_graph						(bool m[][NO_OF_VERTICES], unsigned int size, adj_list_t **g);
void show_graph						(adj_list_t *g);
void show_edge						(adj_list_t *g);
void show_graph_with_weight			(adj_list_t *g);

void breadth_first_search			(adj_list_t *g);
void bfs_traversal					(adj_list_t *g);
void print_bfs_paths				(adj_list_t *g);
void depth_first_search				(adj_list_t *g);
void dfs_traversal					(adj_list_t *g);
void print_dfs_paths				(adj_list_t *g);
void diameter						(adj_list_t *g);
void graph_analyzer					(adj_list_t *g);

#endif /* GRAPH_H_ */
