/*
 * set.h
 *
 *  Created on: 12-Nov-2012
 *      Author: ankitgupta
 */

#ifndef SET_H_
#define SET_H_

#include "graph.h"

typedef unsigned int u32;

void init_set(universal_set_t &u, adj_list_t *g);
unsigned int find_set(universal_set_t &u, unsigned int v);
void union_set(universal_set_t &u, unsigned int a, unsigned int b);

#endif /* SET_H_ */
