/*
 * rb_tree.h
 *
 *  Created on: 09-Oct-2012
 *      Author: ankitgupta
 */

#ifndef RB_TREE_H_
#define RB_TREE_H_

#define INFINITY (1 << 31)
#define NODE_COUNT 10
typedef enum {
    BLACK,
    RED
} color_t;

struct node_t {
    int         key;
    color_t     color;
    node_t      *parent;
    node_t      *left;
    node_t      *right;
};

typedef struct {
    node_t *root;
    unsigned int height;
} tree_t;

extern int test_array[NODE_COUNT];

void create_rb_tree             (tree_t **tree);
void create_rb_node             (node_t **node, int val);

node_t *tree_search_iterative   (node_t *root, int val);
node_t *tree_minimum            (node_t *root);
node_t *tree_maximum            (node_t *root);
node_t *tree_successor          (node_t *root);
node_t *tree_predecessor        (node_t *root);

void build_random_tree          (tree_t *tree, unsigned int node_count);
void inorder_tree_walk          (node_t *root);
void show_tree_sorted           (tree_t *tree);
void left_rotate                (tree_t *tree, node_t *x);
void right_rotate               (tree_t *tree, node_t *y);
void rb_insert                  (tree_t *tree, node_t *z);
void rb_insert_fixup            (tree_t *tree, node_t *z);
void rb_delete                  (tree_t *tree, node_t *z);
void rb_delete_fixup            (tree_t *tree, node_t *x);

#endif /* RB_TREE_H_ */
