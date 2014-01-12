/*
 * list.h
 *
 *  Created on: 02-Oct-2012
 *      Author: ankitgupta
 */

#ifndef LIST_H_
#define LIST_H_

#include <iostream>

struct node_t {
       int data;
       node_t *next;
};

typedef struct {
        node_t *head;
        unsigned int size;
} list_t;

void create_list    (list_t **list);
void delete_list    (list_t **list);
void insert_node    (list_t *list, int data);
void delete_node    (list_t *list, int data);
void show_list      (list_t *list);
void reverse_list   (list_t *list);
list_t *merge_sort  (list_t *list);
list_t *merge       (list_t *left, list_t *right);

#endif /* LIST_H_ */
