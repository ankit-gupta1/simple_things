/*
 * set.cpp
 *
 *  Created on: 12-Nov-2012
 *      Author: ankitgupta
 */

#include <iostream>
#include <cstdlib>
#include "../include/set.h"

using namespace std;

void init_set(universal_set_t &u, adj_list_t *g)
{
    unsigned int i;
    vertex_t *v = NULL;
    u.size = g->size;

    for (i = 0; i < g->size; i++) {
        u.set_list[i].head = NULL;
        u.set_list[i].tail = NULL;
        u.set_list[i].neighbours = 0;
        u.set_addr[i] = 0x00000000;
    }

    for (i = 0; i < g->size; i++) {
        create_vertex(&v, i);
        append_vertex(&u.set_list[i], v);
        u.set_addr[i] = u32(&u.set_list[i]);
        v = NULL;
    }
}

unsigned int find_set(universal_set_t &u, unsigned int v)
{
    unsigned int i = 0;
    vertex_t *vt = NULL;
    list_t *list = NULL;
    for (i = 0; i < u.size; i++) {
        list = &u.set_list[i];
        vt = list->head;
        while (vt) {
            if (vt->val == int(v))
                return u32(&u.set_list[i]);
            else
                vt = vt->next;
        }
    }
    return 0x0;
}

void union_set(universal_set_t &u, unsigned int a, unsigned int b)
{
    vertex_t *vt1 = NULL;
    vertex_t *vt2 = NULL;
    list_t *list1 = NULL;
    list_t *list2 = NULL;
    bool is_present = false;

    list1 = (list_t *)u.set_addr[a];
    vt1 = list1->head;
    list2 = (list_t *)u.set_addr[b];
    vt2 = list2->head;

    while (vt2) {
        while (vt1) {
            if (vt1->val == vt2->val) {
                is_present = true;
            }
            vt1 = vt1->next;
        }

        if (!is_present) {
            vt1 = list1->tail;
            u.set_addr[vt2->val] = u.set_addr[a];
            list2->head = vt2->next;
            if (!list2->head)
                list2->tail = NULL;

            vt2->next = NULL;
            vt1->next = vt2;
            list1->tail = vt2;
        }

        vt2 = list2->head;
        vt1 = list1->head;
        is_present = false;
    }
    list1->tail->next = NULL;
}
