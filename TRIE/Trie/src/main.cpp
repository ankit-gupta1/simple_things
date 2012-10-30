/*
 * main.cpp
 *
 *  Created on: 22-Oct-2012
 *      Author: ankitgupta
 */

#include <iostream>
#include "../include/trie.h"

using namespace std;

int main()
{
	trie_t *trie = NULL;

	string a = "cat";
	string b = "call";

	create_trie(&trie);
	add_word(trie, a);
	add_word(trie, b);
	add_word(trie, "trance");
	add_word(trie, "traffic");
	add_word(trie, "tryst");
	delete_word(trie, a);
	delete_word(trie, "trance");
	cout<<search_word(trie, "call")<<endl;
	cout<<search_word(trie, "try")<<endl;
	return 0;
}
