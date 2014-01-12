/*
 * avl.cpp
 *
 *  Created on: 11-Oct-2012
 *      Author: ankitgupta
 */

#include <iostream>
#include <cstdlib>
#include <ctime>
#include "../include/avl.h"

using namespace std;

#define ABS(x) ((x > 0) ? x : -x)

int test_array[NODE_COUNT];

void create_avl_tree (avl_tree_t **tree)
{
    if (!*tree) {
        (*tree) = (avl_tree_t *)malloc(sizeof(avl_tree_t));
        (*tree)->root = NULL;
        (*tree)->height = 0;
    }
}

void create_avl_node(avl_node_t **node, int val)
{
    if (!*node) {
        *node = (avl_node_t *)malloc(sizeof(avl_node_t));
        (*node)->key = val;
        (*node)->bf = EQUAL;
        (*node)->left = NULL;
        (*node)->right = NULL;
        (*node)->parent = NULL;
    }
}

avl_node_t *tree_search_iterative(avl_node_t *root, int val)
{
    avl_node_t *node = NULL;
    node = root;
    while (node) {
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

avl_node_t *tree_minimum(avl_node_t *root)
{
    avl_node_t *node = NULL;
    node = root;
    while (node->left)
        node = node->left;

    return node;
}

avl_node_t *tree_maximum(avl_node_t *root)
{
    avl_node_t *node = NULL;
    node = root;
    while (node->right)
        node = node->right;

    return node;
}

avl_node_t *tree_successor(avl_node_t *root)
{
    avl_node_t *node = NULL, *node_p = NULL;
    node = root;
    if (!node)
        return node;

    if (node->right)
        return tree_minimum(node->right);

    node_p = node->parent;
    while (node_p) {
        if (node_p->right == node)
            break;

        node = node_p;
        node_p = node_p->parent;
    }
    return node_p;
}

avl_node_t *tree_predecessor(avl_node_t *root)
{
    avl_node_t *node = NULL, *node_p = NULL;
    node = root;
    if (!node)
        return node;

    if (node->left)
        return tree_maximum(node->left);

    node_p = node->parent;
    while (node_p) {
        if (node_p->left == node)
            break;

        node = node_p;
        node_p = node_p->parent;
    }
    return node_p;
}

/*
 * Re-basing node's balance factors after
 * performing rotations.
 */

void rebase_node(avl_node_t *x)
{
    int bl_ft = 0;

    if (x->left && !x->right) {
        if ((!x->left->bf) && x->left->left &&
            x->left->right)
            bl_ft++;

        bl_ft += (ABS(x->left->bf) + 1);
    } else if (!x->left && x->right) {
        if ((!x->right->bf) && x->right->left &&
            x->right->right)
            bl_ft--;

        bl_ft -= (ABS(x->right->bf) + 1);
    } else if (x->left && x->right) {
        if ((!x->left->bf) && x->left->left &&
            x->left->right)
            bl_ft++;

        if ((!x->right->bf) && x->right->left &&
            x->right->right)
            bl_ft--;

        bl_ft += (ABS(x->left->bf) - ABS(x->right->bf));
    }

    x->bf = bl_ft;
}

/*
 * Same as Re-basing nodes operation, with
 * the difference of re-basing the nodes of
 * tree up to root in this case.
 */

void rebase_balancing_factor(avl_node_t *y)
{
    avl_node_t *z = NULL;
    z = y->parent;
    int bl_ft;

    while (z) {
        bl_ft = 0;

        if ((z->bf < 2) && (z->bf > -2)) {

            if ((!z->left->bf) && z->left->left &&
                z->left->right)
                bl_ft++;

            if ((!z->right->bf) && z->right->left &&
                z->right->right)
                bl_ft--;

        bl_ft += (ABS(z->left->bf) - ABS(z->right->bf));
        z->bf = bl_ft;

        } else {

            if (z->bf > 1)
                z->bf--;
            else if (z->bf < -1)
                z->bf++;
        }

        y = z;
        z = z->parent;
    }
}

void left_rotate(avl_tree_t *tree, avl_node_t *x)
{
    avl_node_t *y = NULL;
    if (!x)
        return;

    if (!x->right)
        return;

    y = x->right;
    x->right = y->left;

    if (y->left)
        y->left->parent = x;

    y->parent = x->parent;

    if (!x->parent)
        tree->root = y;
    else if (x == x->parent->left)
        x->parent->left = y;
    else
        x->parent->right = y;

    y->left = x;
    x->parent = y;

    rebase_node(x);
    rebase_node(y);
}

void right_rotate(avl_tree_t *tree, avl_node_t *y)
{
    avl_node_t *x = NULL;
    if (!y)
        return;

    if (!y->left)
        return;

    x = y->left;
    y->left = x->right;

    if (x->right)
        x->right->parent = y;

    x->parent = y->parent;

    if (!y->parent)
        tree->root = x;
    else if (y == y->parent->right)
        y->parent->right = x;
    else
        y->parent->left = x;

    x->right = y;
    y->parent = x;

    rebase_node(y);
    rebase_node(x);
}

/*
 * AVL Tree Insertion algorithm:
 * This algorithm follows general three step
 * procedure. Following points can be noted in
 * this algorithm.
 *
 * 1) Insertion is similar to Binary search tree,
 *    but afterwards we have to do fix-up so as to
 *    make sure AVL tree height balancing properties
 *    followed by a re-basing of node's balancing
 *    factors down the path.
 *
 * 2) Fix-up algorithm is applied only when there is
 *    increment in the height of tree. This means that
 *    Tree is assumed to be following AVL properties
 *    before and after insertion.
 *
 * 3) Re-basing algorithm is always applied. In case
 *    if there's a fix-up required, re-basing will
 *    be done afterwards.
 */

void avl_insert(avl_tree_t *tree, avl_node_t *z)
{
    avl_node_t *x = NULL;
    avl_node_t *y = NULL;
    unsigned int depth = 1;
    bool update = false;
    x = tree->root;

    while (x) {
        y = x;
        depth++;
        if (z->key < x->key) {
            x->bf++;
            x = x->left;
        } else {
            x->bf--;
            x = x->right;
        }
    }

    if (tree->height < depth) {
        tree->height = depth;
        update = true;
    }

    z->parent = y;
    if (!y)
        tree->root = z;
    else if (z->key < y->key)
        y->left = z;
    else
        y->right = z;

    z->left = NULL;
    z->right = NULL;

    if (update)
        avl_insert_fixup(tree, z);
    else if (y->parent)
        rebase_balancing_factor(y);
}


void avl_insert_fixup(avl_tree_t *tree, avl_node_t *x)
{
    avl_node_t *y = NULL;
    avl_node_t *z = NULL;
    bool update = false;
    if (!x->parent)
        return;

    y = x->parent;
    if (!y->parent)
        return;

    while (y->parent && !update) {
        if (y->parent->bf < LESSER) {
            if (x == y->left) {
                right_rotate(tree, y);
                z = x;
                x = y;
                y = z;
            }

            left_rotate(tree, y->parent);
            update = true;
            tree->height--;
        } else if (y->parent->bf > GREATER) {
            if (x == y->right) {
                left_rotate(tree, y);
                z = x;
                x = y;
                y = z;
            }

            right_rotate(tree, y->parent);
            update = true;
            tree->height--;
        }

        x = y;
        y = y->parent;
        if (!y)
            break;
    }

    if (y && update)
        rebase_balancing_factor(x);
}

void build_random_tree (avl_tree_t *tree, unsigned int node_count)
{
    unsigned int i;
    avl_node_t *node = NULL;
    if (!tree)
        return;

    for (i = 0; i < node_count; i++) {
        srand(time(NULL) * i + i * i + 7);
        test_array[i] = (rand() % 100 + 1);
        create_avl_node(&node, test_array[i]);
        avl_insert(tree, node);
        node = NULL;
        free(node);
    }
}

void inorder_tree_walk(avl_node_t *root)
{
    if (root) {
        inorder_tree_walk(root->left);
        cout<<root->key<<" ";
        inorder_tree_walk(root->right);
    }
}

void show_tree_sorted(avl_tree_t *tree)
{
    cout<<"\nTree in sorted order :\n";
    inorder_tree_walk(tree->root);
    cout<<"\n";
}

/*
 * FIX ME
 * Correct tree height while deletion.
 */

void avl_delete(avl_tree_t *tree, avl_node_t *z)
{
    avl_node_t *x = NULL;
    avl_node_t *y = NULL;
    int root_bf = tree->root->bf;

    if (!z->left || !z->right)
        y = z;
    else
        y = tree_successor(z);

    if (y->left)
        x = y->left;
    else
        x = y->right;

    if (x)
        x->parent = y->parent;

    if (!y->parent)
        tree->root = x;
    else if (y == y->parent->left)
        y->parent->left = x;
    else
        y->parent->right = x;

    if (y != z)
        z->key = y->key;

    if (x)
        avl_delete_fixup(tree, x, root_bf);
    else if (y->parent)
        avl_delete_fixup(tree, y->parent, root_bf);
    else
        tree->height--;
}


void avl_delete_fixup(avl_tree_t *tree, avl_node_t *x, int root_bf)
{
    avl_node_t *y = NULL;
    avl_node_t *z = NULL;
    avl_node_t *u = NULL;
    bool is_rotated = false;

    rebase_node(x);

    while (x) {
        if (y && !is_rotated)
            if (x->left == y)
                x->bf--;
            else
                x->bf++;
        else if (is_rotated)
            rebase_node(z);

        if (x->bf < -1) {
            if (x->right->left && !x->right->right) {
                z = x->right;
                right_rotate(tree, z);
            }

            z = x;
            left_rotate(tree, z);
            is_rotated = true;
        } else if (x->bf > 1) {
            if (!x->left->left && x->left->right) {
                z = x->left;
                left_rotate(tree, z);
            }

            z = x;
            right_rotate(tree, z);
            is_rotated = true;
        } else if (is_rotated)
            rebase_node(x);

        y = x;
        x = x->parent;
    }

    if (!tree->root->bf && ABS(root_bf))
        tree->height--;
}
