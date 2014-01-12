/*
 * rb_tree.cpp
 *
 *  Created on: 09-Oct-2012
 *      Author: ankitgupta
 */

#include <iostream>
#include <cstdlib>
#include <ctime>
#include "../include/rb_tree.h"

using namespace std;

int test_array[NODE_COUNT];
extern node_t *NIL;

void init_nil()
{
    NIL = (node_t *)malloc(sizeof(node_t));
    NIL->color = BLACK;
    NIL->key = INFINITY;
    NIL->left = NULL;
    NIL->right = NULL;
    NIL->parent = NULL;
}

void create_rb_tree(tree_t **tree)
{
    init_nil();
    if (!*tree) {
        (*tree) = (tree_t *)malloc(sizeof(tree_t));
        (*tree)->root = NIL;
        (*tree)->height = 0;
    }
}

void create_rb_node(node_t **node, int val)
{
    *node = (node_t *)malloc(sizeof(node_t));
    (*node)->key = val;
    (*node)->color = BLACK;
    (*node)->left = NIL;
    (*node)->right = NIL;
    (*node)->parent = NIL;
}

node_t *tree_search_iterative(node_t *root, int val)
{
    node_t *node = NIL;
    node = root;
    while (node != NIL) {
        if (node->key == val) {
            break;
        }
        if (val < node->key)
            node = node->left;
        else
            node = node->right;
    }
    return node;
}

node_t *tree_minimum(node_t *root)
{
    node_t *node = NIL;
    node = root;
    while (node->left != NIL)
        node = node->left;

    return node;
}

node_t *tree_maximum(node_t *root)
{
    node_t *node = NIL;
    node = root;
    while (node->right != NIL)
        node = node->right;

    return node;
}

node_t *tree_successor(node_t *root)
{
    node_t *node = NIL, *node_p = NULL;
    node = root;
    if (node == NIL)
        return node;

    if (node->right != NIL)
        return tree_minimum(node->right);

    node_p = node->parent;
    while (node_p != NIL) {
        if (node_p->right == node)
            break;

        node = node_p;
        node_p = node_p->parent;
    }
    return node_p;
}

node_t *tree_predecessor(node_t *root)
{
    node_t *node = NIL, *node_p = NULL;
    node = root;
    if (node == NIL)
        return node;

    if (node->left != NIL)
        return tree_maximum(node->left);

    node_p = node->parent;
    while (node_p != NIL) {
        if (node_p->left == node)
            break;

        node = node_p;
        node_p = node_p->parent;
    }
    return node_p;
}

/*********************************
 *      x                   y    *
 *     / \      LEFT       / \   *
 *    /   \   ROTATION    /   \  *
 *  a      y    ==>>     x     c *
 *        / \           / \      *
 *       /   \         /   \     *
 *      b     c       a     b    *
 *                               *
 *********************************/

void left_rotate(tree_t *tree, node_t *x)
{
    node_t *y = NIL;
    if (x == NIL)
        return;

    if (x->right == NIL)
        return;

    y = x->right;
    x->right = y->left;
    y->left->parent = x;
    y->parent = x->parent;

    if (x->parent == NIL)
        tree->root = y;
    else if (x == x->parent->left)
        x->parent->left = y;
    else
        x->parent->right = y;

    y->left = x;
    x->parent = y;
}

void right_rotate(tree_t *tree, node_t *y)
{
    node_t *x = NIL;
    if (y == NIL)
        return;

    if (y->left == NIL)
        return;

    x = y->left;
    y->left = x->right;
    x->right->parent = y;
    x->parent = y->parent;

    if (y->parent == NIL)
        tree->root = x;
    else if (y == y->parent->right)
        y->parent->right = x;
    else
        y->parent->left = x;

    x->right = y;
    y->parent = x;
}


void rb_insert(tree_t *tree, node_t *z)
{
    node_t *y = NIL;
    node_t *x = tree->root;
    while (x != NIL) {
        y = x;
        if (z->key < x->key)
            x = x->left;
        else
            x = x->right;
    }

    z->parent = y;
    if (y == NIL)
        tree->root = z;
    else if (z->key < y->key)
        y->left = z;
    else
        y->right = z;

    z->color = RED;
    z->left = NIL;
    z->right = NIL;
    rb_insert_fixup(tree, z);
}

void rb_insert_fixup(tree_t *tree, node_t *z)
{
    node_t *y = NIL;
    while (z->parent->color == RED) {
        if (z->parent->parent == NIL)
            break;

        if (z->parent == z->parent->parent->left) {
            y = z->parent->parent->right;
            if (y->color == RED) {
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            } else if (z == z->parent->right) {
                z = z->parent;
                left_rotate(tree, z);
            } else {
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                right_rotate(tree, z->parent->parent);
            }
        } else {
            y = z->parent->parent->left;
            if (y->color == RED) {
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            } else if (z == z->parent->left) {
                z = z->parent;
                right_rotate(tree, z);
            } else {
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                left_rotate(tree, z->parent->parent);
            }
        }
    }
    tree->root->color = BLACK;
}

void rb_delete(tree_t *tree, node_t *z)
{
    node_t *x = NIL;
    node_t *y = NIL;

    if ((z->left == NIL) || (z->right == NIL))
        y = z;
    else
        y = tree_successor(z);

    if (y->left != NIL)
        x = y->left;
    else
        x = y->right;

    x->parent = y->parent;
    if (y->parent == NIL)
        tree->root = x;
    else if (y == y->parent->left)
        y->parent->left = x;
    else
        y->parent->right = x;

    if (y != z)
        z->key = y->key;

    if (y->color == BLACK)
        rb_delete_fixup(tree, x);
}


void rb_delete_fixup(tree_t *tree, node_t *x)
{
    node_t *w = NIL;

    while ((x != tree->root) && (x->color == BLACK)) {
        if (x == x->parent->left) {
            w = x->parent->right;
            if (w->color == RED) {
                w->color = BLACK;
                x->parent->color =RED;
                left_rotate(tree, x->parent);
                w = x->parent->right;
            }

            if ((w->left->color == BLACK) &&
                (w->right->color == BLACK)) {
                w->color = RED;
                x = x->parent;
            } else if (w->right->color == BLACK) {
                w->left->color = BLACK;
                w->color = RED;
                right_rotate(tree, w);
                w = x->parent->right;
            } else {
                w->color = x->parent->color;
                x->parent->color = BLACK;
                w->right->color = BLACK;
                left_rotate(tree, x->parent);
                x = tree->root;
            }
        } else {
            w = x->parent->left;
            if (w->color == RED) {
                w->color = BLACK;
                x->parent->color =RED;
                right_rotate(tree, x->parent);
                w = x->parent->left;
            }

            if ((w->right->color == BLACK) &&
                (w->left->color == BLACK)) {
                w->color = RED;
                x = x->parent;
            } else if (w->left->color == BLACK) {
                w->right->color = BLACK;
                w->color = RED;
                left_rotate(tree, w);
                w = x->parent->left;
            } else {
                w->color = x->parent->color;
                x->parent->color = BLACK;
                w->left->color = BLACK;
                right_rotate(tree, x->parent);
                x = tree->root;
            }
        }
    }
}

void build_random_tree (tree_t *tree, unsigned int node_count)
{
    unsigned int i;
    node_t *node;
    if (!tree)
        return;

    for (i = 0; i < node_count; i++) {
        srand(time(NULL) * i + i * i + 7);
        test_array[i] = (rand() % 100 + 1);
        create_rb_node(&node, test_array[i]);
        rb_insert(tree, node);
        node = NULL;
        free(node);
    }
}

void inorder_tree_walk(node_t *root)
{
    if (root != NIL) {
        inorder_tree_walk(root->left);
        cout<<root->key<<" ";
        inorder_tree_walk(root->right);
    }
}

void show_tree_sorted(tree_t *tree)
{
    cout<<"\nTree in sorted order :\n";
    inorder_tree_walk(tree->root);
    cout<<"\n";
}
