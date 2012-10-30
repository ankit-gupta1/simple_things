/*
 * mergesort.cpp
 *
 *  Created on: 02-Oct-2012
 *      Author: ankitgupta
 */

#include <iostream>
#include <cstdlib>
#include "../include/list.h"
using namespace std;

list_t *merge_sort(list_t *list)
{
	list_t *left = NULL;
	list_t *right = NULL;
	node_t *curr_node = NULL;
	unsigned int middle;
	unsigned int i;

	if (!list)
		return list;

	create_list(&left);
	create_list(&right);
	middle = list->size >> 1;
	curr_node = list->head;

	for(i = 0; i < middle; i++) {
		insert_node(left, curr_node->data);
		curr_node = curr_node->next;
	}

	for(i = middle; i < list->size ; i++) {
		insert_node(right, curr_node->data);
		curr_node = curr_node->next;
	}

	i = list->size;
	delete_list(&list);

	if (i > 1) {
		left = merge_sort(left);
		right = merge_sort(right);
	}

	return merge(left, right);
}

list_t *merge(list_t* left, list_t* right)
{
	list_t* result = NULL;
	create_list(&result);

	if (left && !right) {
		while (left->size > 0) {
			insert_node(result, left->head->data);
			delete_node(left, left->head->data);
		}
	} else if (!left && right) {
		while (right->size > 0) {
			insert_node(result, right->head->data);
			delete_node(right, right->head->data);
		}
	} else if (left && right) {
		while ((left->size > 0) || (right->size > 0)) {
			if ((left->size > 0) && (right->size > 0)) {
				if (left->head->data <= right->head->data) {
					insert_node(result, left->head->data);
					delete_node(left, left->head->data);
				} else {
					insert_node(result, right->head->data);
					delete_node(right, right->head->data);
				}
			} else if (left->size > 0) {
				insert_node(result, left->head->data);
				delete_node(left, left->head->data);
			} else if (right->size > 0) {
				insert_node(result, right->head->data);
				delete_node(right, right->head->data);
			}
		}
	}

	return result;
}
