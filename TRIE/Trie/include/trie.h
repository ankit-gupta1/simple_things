/*
 * trie.h
 *
 *  Created on: 22-Oct-2012
 *      Author: ankitgupta
 */

#ifndef TRIE_H_
#define TRIE_H_

#include <iostream>
#include <cstdlib>
#include <string>

using namespace std;

struct trie_node_t {
    char key;
    unsigned int no_of_kids;
    trie_node_t * child;
    bool mark_word;
};

struct trie_t {
    trie_node_t *root;
};

void create_node            (trie_node_t **node, char ch);
void create_trie            (trie_t **trie);
trie_node_t* search_child   (trie_node_t *node, char c);
void add_word               (trie_t *trie, string s);
void delete_word            (trie_t *trie, string s);
bool search_word            (trie_t *trie, string s);

#endif /* TRIE_H_ */
