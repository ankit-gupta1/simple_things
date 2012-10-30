/*
 * avl.h
 *
 *  Created on: 11-Oct-2012
 *      Author: ankitgupta
 */

#ifndef AVL_H_
#define AVL_H_

#define NODE_COUNT 10

typedef enum {
	LESSER = -1,
	EQUAL = 0,
	GREATER,
} bf_t;

struct avl_node_t {
	int key;
	int bf; /* Balancing Factor */
	avl_node_t *left;
	avl_node_t *right;
	avl_node_t *parent;
};

typedef struct {
	avl_node_t *root;
	unsigned int height;
} avl_tree_t;

extern int test_array[NODE_COUNT];

void create_avl_tree			(avl_tree_t **tree);
void create_avl_node			(avl_tree_t **node, int val);

avl_node_t *tree_minimum			(avl_node_t *root);
avl_node_t *tree_maximum			(avl_node_t *root);
avl_node_t *tree_successor			(avl_node_t *root);
avl_node_t *tree_predecessor		(avl_node_t *root);
avl_node_t *tree_search_iterative		(avl_node_t *root, int val);

void build_random_tree			(avl_tree_t *tree, unsigned int node_count);
void inorder_tree_walk			(avl_node_t *root);
void show_tree_sorted			(avl_tree_t *tree);
void avl_left_rotate			(avl_tree_t *tree, avl_node_t *x);
void avl_right_rotate			(avl_tree_t *tree, avl_node_t *y);
void avl_insert					(avl_tree_t *tree, avl_node_t *z);
void avl_insert_fixup			(avl_tree_t *tree, avl_node_t *z);
void avl_delete					(avl_tree_t *tree, avl_node_t *z);
void avl_delete_fixup			(avl_tree_t *tree, avl_node_t *x, int root_bf);
#endif /* AVL_H_ */
