/*
 * bst.h
 *
 *  Created on: 06-Oct-2012
 *      Author: ankitgupta
 */

#ifndef BST_H_
#define BST_H_

#define NODE_COUNT 80

struct node_t {
    int key;
    node_t *parent;
    node_t *left;
    node_t *right;
};

struct node_list_t {
    node_t *node;
    node_list_t *next;
    node_list_t *prev;
};

typedef struct {
    node_t *root;
    unsigned int height;
} tree_t;

typedef node_list_t node_stack_t;
typedef node_list_t node_queue_t;

/**
 * Binary search tree API's.
 */

extern int test_array[NODE_COUNT];

void create_tree            (tree_t **tree);
void free_tree              (tree_t **tree);
void create_node            (node_t **node, int val);
void inorder_tree_walk      (node_t *root);
void preorder_tree_walk     (node_t *root);
void postorder_tree_walk    (node_t *root);
void tree_insertion         (tree_t *tree, node_t *node_z);
void tree_deletion          (tree_t *tree, node_t *node_z);
void build_random_tree      (tree_t *tree, unsigned int node_count);
void show_tree_sorted       (tree_t *tree);
void show_tree_preorder     (tree_t *tree);
void show_tree_postorder    (tree_t *tree);

void inorder_tree_walk_iterative            (tree_t *tree);
void morris_traversal                       (tree_t *tree);
void inorder_tree_walk_iterative_wo_stack   (tree_t *tree);
void preorder_tree_walk_iterative           (tree_t* tree);
void preorder_tree_walk_iterative_wo_stack  (tree_t *tree);
void postorder_tree_walk_iterative          (tree_t* tree);
void postorder_tree_walk_iterative2         (tree_t* tree);
void postorder_tree_walk_iterative_wo_stack (tree_t *tree);
void level_order_tree_traversal             (tree_t* tree);
void lowest_common_ancestor                 (tree_t *tree, int n1, int n2);

int tree_height             (node_t *node);
int tree_height_iterative   (tree_t *tree);

node_t *tree_search             (node_t *root, int val);
node_t *tree_search_iterative   (node_t *root, int val);
node_t *tree_minimum            (node_t *root);
node_t *tree_maximum            (node_t *root);
node_t *tree_successor          (node_t *root);
node_t *tree_predecessor        (node_t *root);

#endif /* BST_H_ */
