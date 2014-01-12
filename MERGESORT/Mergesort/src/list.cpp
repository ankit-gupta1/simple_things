/*
 * list.cpp
 *
 *  Created on: 02-Oct-2012
 *      Author: ankitgupta
 */

#include <iostream>
#include <cstdlib>
#include "../include/list.h"
using namespace std;

void create_list (list_t **list)
{
    if (*list) {
        return;
    }

    *list = (list_t *)malloc(sizeof(list_t));
    (*list)->head = NULL;
    (*list)->size = 0;
}

void delete_list (list_t **list)
{
    node_t *curr_node;
    if (*list) {
        while ((*list)->head) {
            curr_node = (*list)->head;
            (*list)->head = (*list)->head->next;
            free(curr_node);
            (*list)->size--;
        }
        free(*list);
        *list = NULL;
    }
}

void insert_node (list_t *list, int data)
{
    node_t *curr_node, *new_node;
    if (!list)
        return;

    new_node = (node_t *)malloc(sizeof(node_t));
    new_node->data = data;
    new_node->next = NULL;

    if (list->head == NULL) {
        list->head = new_node;
    } else {
        curr_node = list->head;
        while (curr_node->next) {
            curr_node = curr_node->next;
        }
        curr_node->next = new_node;
    }
    list->size++;
}

void delete_node (list_t *list, int data)
{
    node_t *curr_node, *prev_node;

    if (!list)
        return;

    prev_node = NULL;
    curr_node = list->head;
    while ((curr_node->data != data) &&
            curr_node->next) {
        prev_node = curr_node;
        curr_node = curr_node->next;
    }

    if (curr_node->data == data) {
        if (curr_node == list->head) {
            list->head = list->head->next;
        } else if (!curr_node->next) {
            prev_node->next = NULL;
        } else {
            prev_node->next = curr_node->next;
        }
        list->size--;
        free(curr_node);
    } else {
        return;
    }
}

void show_list(list_t *list)
{
    if (!list) {
        cout<<"\nList is empty\n";
    } else {
        node_t *curr_node;
        curr_node = list->head;
        cout<<"\nCurrent list is this:\n";

        while (curr_node) {
            cout<<curr_node->data<<" ";
            curr_node = curr_node->next;
        }
        cout<<"\n";
    }
}

void reverse_list(list_t *list)
{
    node_t *prev_node, *curr_node, *next_node;
    if (!list) {
        cout<<"List is empty\n";
    } else {
        prev_node = NULL;
        curr_node = list->head;
        next_node = list->head->next;
        while (next_node) {
            curr_node->next = prev_node;
            prev_node = curr_node;
            curr_node = next_node;
            next_node = next_node->next;
        }
        curr_node->next = prev_node;
        list->head = curr_node;
    }
}
