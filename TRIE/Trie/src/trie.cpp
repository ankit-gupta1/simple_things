/*
 * trie.cpp
 *
 *  Created on: 22-Oct-2012
 *      Author: ankitgupta
 */
#include <iostream>
#include <cstdlib>
#include "../include/trie.h"

using namespace std;

void create_node(trie_node_t **node, char ch)
{
    if (!*node) {
        (*node) = (trie_node_t *)malloc(sizeof(trie_node_t));
        (*node)->key = ch;
        (*node)->child = NULL;
        (*node)->no_of_kids = 0;
        (*node)->mark_word = false;
    }
}

void create_trie(trie_t **trie)
{
    if (!*trie) {
        (*trie) = (trie_t *)malloc(sizeof(trie_node_t));
        (*trie)->root = NULL;
        create_node(&(*trie)->root, ' ');
    }
}

void create_child(trie_node_t **node, char ch)
{
    unsigned int i = 0;

    if (*node) {
        if (!(*node)->child) {
            (*node)->child = (trie_node_t *)malloc(sizeof(trie_node_t));
            (*node)->no_of_kids = 1;
            (*node)->child[0].key = ch;
            (*node)->child[0].child = NULL;
            (*node)->child[0].no_of_kids = 0;
            (*node)->child[0].mark_word = false;
        } else {
            i = (*node)->no_of_kids + 1;
            (*node)->child = (trie_node_t *)realloc((*node)->child, i * sizeof(trie_node_t));
            (*node)->no_of_kids++;
            (*node)->child[i - 1].key = ch;
            (*node)->child[i - 1].child = NULL;
            (*node)->child[i - 1].no_of_kids = 0;
            (*node)->child[i - 1].mark_word = false;
        }
    }
}

void free_child(trie_node_t **p, trie_node_t **c)
{
    unsigned int i = 0;
    unsigned int j = 0;

    for (i = 0; i < (*p)->no_of_kids; i++) {
        if ((*p)->child[i].key == (*c)->key)
            break;
    }

    for (j = (i + 1); j < (*p)->no_of_kids; j++) {
        (*p)->child[j - 1].key = (*p)->child[j].key;
        (*p)->child[j - 1].child = (*p)->child[j].child;
        (*p)->child[j - 1].mark_word = (*p)->child[j].mark_word;
        (*p)->child[j - 1].no_of_kids = (*p)->child[j].no_of_kids;
    }

    (*p)->no_of_kids--;
}

trie_node_t* search_child(trie_node_t *node, char c)
{
    trie_node_t *curr = NULL;
    unsigned int i;

    if (!node)
        return curr;

    for (i = 0; i < node->no_of_kids; i++) {
        if (node->child[i].key == c) {
            curr = &node->child[i];
            break;
        }
    }

    return curr;
}

void add_word(trie_t *trie, string s)
{
    trie_node_t *curr = NULL;
    trie_node_t *kids = NULL;

    unsigned int i;
    if (!trie)
        return;

    if (!s.length())
        return;

    curr = trie->root;
    for (i = 0; i < s.length(); i++) {
        kids = search_child(curr, s[i]);
        if (kids)
            curr = kids;
        else {
            create_child(&curr, s[i]);
            curr = &curr->child[curr->no_of_kids - 1];
        }
    }

    curr->mark_word = true;
}

bool search_word(trie_t *trie, string s)
{
    trie_node_t *curr = NULL;
    trie_node_t *kids = NULL;

    unsigned int match = 0;
    unsigned int i;

    if (!trie)
        return false;

    if (!s.length())
        return false;

    curr = trie->root;
    for (i = 0; i < s.length(); i++) {
        kids = search_child(curr, s[i]);
        if (kids) {
            curr = kids;
            match++;
        } else
            break;
    }

    if ((match == s.length()) && curr->mark_word)
        return true;
    else
        return false;
}

void delete_word(trie_t *trie, string s)
{
    trie_node_t **curr = NULL;
    trie_node_t *kids = NULL;

    unsigned int match = 0;
    unsigned int i;

    if (!trie)
        return;

    if (!s.length())
        return;

    curr = new trie_node_t*[s.length() + 1];
    curr[0] = trie->root;
    for (i = 0; i < s.length(); i++) {
        kids = search_child(curr[i], s[i]);
        if (kids) {
            curr[i + 1] = kids;
            match++;
        } else
            break;
    }

    if (match == s.length()) {
        for (i = match; i > 0; i--) {
            if (curr[i]->no_of_kids) {
                curr[i]->mark_word = false;
                break;
            } else {
                free_child(&curr[i - 1], &curr[i]);
            }
        }
    }

    delete [] curr;
}



