/*
 * graph.cpp
 *
 *  Created on: 23-Oct-2012
 *      Author: ankitgupta
 */

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <new>
#include <iomanip>
#include "../include/graph.h"
#include "../include/queue.h"
#include "../include/set.h"

using namespace std;

static unsigned int time_count;
static unsigned int elem_count;

void init_matrix(bool M[][NO_OF_VERTICES], unsigned int size)
{
    unsigned int i, j;

    for (i = 0; i < size; i++) {
        for (j = 0; j < size; j++) {
            M[i][j] = false;
        }
    }
}

void init_weight(unsigned int W[][NO_OF_VERTICES], unsigned int size)
{
    unsigned int i, j;

    for (i = 0; i < size; i++) {
        for (j = 0; j < size; j++) {
            W[i][j] = 0;
        }
    }
}

void create_matrix(bool M[][NO_OF_VERTICES], list_edge_t *list)
{
    edge_t *e = NULL;
    e = list->head;
    while (e) {
        M[e->u][e->v] = true;
        M[e->v][e->u] = true;
        e = e->next;
    }
}

void create_weight(unsigned int W[][NO_OF_VERTICES], list_edge_t *list)
{
    edge_t *e = NULL;
    e = list->head;
    while (e) {
        W[e->u][e->v] = e->w;
        W[e->v][e->u] = e->w;
        e = e->next;
    }
}

void populate_matrix_undirected(bool M[][NO_OF_VERTICES], unsigned int size)
{
    unsigned int i, j;

    for (i = 0; i < size; i++) {
        for (j = i; j < size; j++) {
            srand(time(NULL) + i * j * 7 + 100 *j);
            M[i][j] = (rand() % 2) ? true : false;
            M[j][i] = M[i][j];
        }
    }
}

void populate_matrix_directed(bool M[][NO_OF_VERTICES], unsigned int size)
{
    unsigned int i, j;

    for (i = 0; i < size; i++) {
        for (j = 0; j < size; j++) {
            srand(time(NULL) + i * j * 7 + (1900 *j));
            if (!M[i][j])
                M[i][j] = (rand() % 2) ? true : false;
        }
    }
}

void populate_weight(bool M[][NO_OF_VERTICES],
        unsigned int W[NO_OF_VERTICES][NO_OF_VERTICES],
        unsigned int size, bool directed)
{
    unsigned int i, j;

    if (!directed) {
        for (i = 0; i < size; i++) {
            for (j = i; j < size; j++) {
                srand(time(NULL) + i * j * 7 + 100 * j);
                if (M[i][j])
                    W[i][j] = 1 + (rand() % 20);
                else
                    W[i][j] = 0;

                W[j][i] = W[i][j];
            }
        }
    } else {
        for (i = 0; i < size; i++) {
            for (j = 0; j < size; j++) {
                srand(time(NULL) + i * j * 7 + 100 * j);
                if (M[i][j])
                    W[i][j] = 1 + (rand() % 20);
                else
                    W[i][j] = 0;
            }
        }
    }
}

void show_matrix(bool M[][NO_OF_VERTICES], unsigned int size)
{
    unsigned int i, j;

    cout<<"\nAdjacency matrix is :\n";
    for (i = 0; i < size; i++) {
        for (j = 0; j < size; j++) {
            cout<<" "<<M[i][j];
        }
        cout<<"\n";
    }
}

void create_vertex(vertex_t **v, int val)
{
    if (!*v) {
        *v = new vertex_t;
        (*v)->val = val;
        (*v)->next = NULL;
    }
}

void create_edge(edge_t **e, int u, int v)
{
    if (!*e) {
        *e = new edge_t;
        srand(time(NULL) + u * u * 7 + 100 *v);
        (*e)->u = u;
        (*e)->v = v;
        (*e)->w = 1 + (rand() % 20);
        (*e)->edge_type = NIL;
        (*e)->next = NULL;
    }
}

void create_edge_undirected(edge_t **e, int u, int v, int w)
{
    if (!*e) {
        *e = new edge_t;
        (*e)->u = u;
        (*e)->v = v;
        (*e)->w = w;
        (*e)->edge_type = NIL;
        (*e)->next = NULL;
    }
}

void create_list(list_t **list)
{
    if (!*list) {
        (*list)->head = NULL;
        (*list)->tail = NULL;
        (*list)->neighbours = 0;
    }
}

void append_vertex(list_t *list, vertex_t *v)
{
    vertex_t *curr = NULL;

    if (!list)
        return;

    if (!v)
        return;

    if (!list->head) {
        list->head = v;
        list->tail = v;
    } else {
        curr = list->tail;
        curr->next = v;
        list->tail = v;
    }

    list->neighbours++;
}

void append_edge(list_edge_t *list, edge_t *e)
{
    edge_t *curr = NULL;

    if (!list)
        return;

    if (!e)
        return;

    if (!list->head) {
        list->head = e;
        list->tail = e;
    } else {
        curr = list->tail;
        curr->next = e;
        list->tail = e;
    }
}

void init_bfs_params(bfs_params_t **bfs_params, int size)
{
    (*bfs_params)->bfs_trav = new int[size];
    (*bfs_params)->bfs_q = NULL;
    create_queue(&(*bfs_params)->bfs_q);
}

void init_dfs_params(dfs_params_t **dfs_params, int size)
{
    (*dfs_params)->dfs_trav = new int[size];
    (*dfs_params)->dfs_q = NULL;
    create_queue(&(*dfs_params)->dfs_q);
}

void init_graph(bool M[][NO_OF_VERTICES],
        unsigned int W[NO_OF_VERTICES][NO_OF_VERTICES],
        unsigned int size, adj_list_t **g)
{
    unsigned int i, j;
    vertex_t *v = NULL;
    edge_t *e = NULL;

    *g          = (adj_list_t *)malloc(sizeof(adj_list_t));
    (*g)->graph     = (list_t *)malloc(size * (sizeof(list_t)));
    (*g)->edge      = (list_edge_t *)malloc(size * sizeof(list_edge_t));
    (*g)->bfs_vparams   = (bfs_vparams_t *)malloc(size * sizeof(bfs_vparams_t));
    (*g)->bfs_params    = (bfs_params_t *)malloc(sizeof(bfs_params_t));
    (*g)->dfs_vparams   = (dfs_vparams_t *)malloc(size * sizeof(dfs_vparams_t));
    (*g)->dfs_params    = (dfs_params_t *)malloc(sizeof(dfs_params_t));
    (*g)->sub_graph     = (adj_list_t *)malloc(sizeof(adj_list_t));

    init_bfs_params(&(*g)->bfs_params, size);
    init_dfs_params(&(*g)->dfs_params, size);

    for (i = 0; i < size; i++) {
        (*g)->graph[i].head = NULL;
        (*g)->graph[i].tail = NULL;
        (*g)->graph[i].neighbours = 0;
        (*g)->edge[i].head = NULL;
        (*g)->edge[i].tail = NULL;

        for (j = 0; j < size; j++) {
            if (M[i][j]) {
                create_vertex(&v, j);
                append_vertex(&(*g)->graph[i], v);
                create_edge_undirected(&e, i, j, W[i][j]);
                append_edge(&(*g)->edge[i], e);
                v = NULL;
                e = NULL;
            }
        }
    }

    (*g)->cnctd_cmpnts = 0;
    (*g)->size = size;
}

void show_graph(adj_list_t *g)
{
    unsigned int i;
    vertex_t *v = NULL;

    cout<<"\nAdjacency list is :\n";
    for (i = 0; i < g->size; i++) {
        cout<<"\n["<<i<<"]";
        v = g->graph[i].head;

        while (v) {
            cout<<" -> "<<v->val;
            v = v->next;
        }
        cout<<endl;
    }
}

void show_graph_with_weight(adj_list_t *g)
{
    unsigned int i;
    vertex_t *v = NULL;
    edge_t *e = NULL;

    cout<<"\nAdjacency list of weighted graph is :\n";
    for (i = 0; i < g->size; i++) {
        cout<<"\n["<<i<<"]";
        v = g->graph[i].head;
        e = g->edge[i].head;

        while (v) {
            cout<<" -> "<<v->val<<"("<<e->w<<")";
            v = v->next;
            e = e->next;
        }
        cout<<endl;
    }
}

void show_edge(adj_list_t *g)
{
    unsigned int i;
    edge_t *e = NULL;

    cout<<"\nEdges of Graph are :\n";
    cout<<setw(2)<<"u"<<setw(2)<<"v"<<setw(4)<<"w"<<endl;
    for (i = 0; i < g->size; i++) {
        e = g->edge[i].head;

        while (e) {
            cout<<setw(2)<<e->u<<setw(2)<<e->v<<setw(4)<<e->w<<endl;
            e = e->next;
        }
    }
}

void show_edge_list(adj_list_t *g, edge_t *e)
{
    unsigned int i;
    unsigned int total_edges = 0;

    for (i = 0; i < g->size; i++)
        total_edges += g->graph[i].neighbours;

    cout<<"\nEdges of Graph from list are :\n";
    cout<<setw(2)<<"u"<<setw(2)<<"v"<<setw(4)<<"w"<<endl;
    for (i = 0; i < total_edges; i++) {
        cout<<setw(2)<<e[i].u<<setw(2)<<e[i].v<<setw(4)<<e[i].w<<endl;
    }
}

void breadth_first_search(adj_list_t *g)
{
    unsigned int s, i, j;
    int u, v;
    queue_t *q = NULL;
    queue_t *qt = NULL;
    int *trav = NULL;
    vertex_t *v_v = NULL;
    g->cnctd_cmpnts = 0;

    /*
     * Randomly pick any initial point in a graph.
     */

    srand(time(NULL) +1001);
    s = rand() % g->size;

    for (i = 0; i < g->size; i++) {
        g->bfs_vparams[i].color = WHITE;
        g->bfs_vparams[i].distance = (~(1 << 31));
        g->bfs_vparams[i].pred_index = 1 << 31;
    }

    g->bfs_vparams[s].color = GRAY;
    g->bfs_vparams[s].distance = 0;
    g->bfs_vparams[s].pred_index = 1 << 31;

    create_queue(&q);
    enqueue(q, s);

    qt = g->bfs_params->bfs_q;
    trav = g->bfs_params->bfs_trav;
    i = 0;

    while (q->front || (i < g->size)) {
        u = dequeue(q);
        if (u > -1) {
            v_v = g->graph[u].head;
            trav[i++] = u;

            while (v_v) {
                v = v_v->val;
                if (g->bfs_vparams[v].color == WHITE) {
                    g->bfs_vparams[v].color = GRAY;
                    g->bfs_vparams[v].distance = g->bfs_vparams[u].distance + 1;
                    g->bfs_vparams[v].pred_index = u;
                    enqueue(q, v);
                }

                v_v = v_v->next;
            }
            g->bfs_vparams[u].color = BLACK;
        } else {
            for (j = 0; j < g->size; j++)
                if (g->bfs_vparams[j].color == WHITE)
                    break;

            g->bfs_vparams[j].color = GRAY;
            g->bfs_vparams[j].distance = 0;
            g->bfs_vparams[j].pred_index = 1 << 31;

            enqueue(q, j);
            enqueue(qt, i);
            g->cnctd_cmpnts++;
        }
    }

    enqueue(qt, g->size);

    qt = NULL;
    free_queue(&q);
}

void print_bfs_paths(adj_list_t *g)
{
    unsigned int i, j, k, s;
    queue_t *q = NULL;
    int *trav = NULL;

    if (g) {
        q = g->bfs_params->bfs_q;
        trav = g->bfs_params->bfs_trav;
        if (queue_front(q) > -1) {
            j = 0;

            for (i = 0; i <= g->cnctd_cmpnts; i++) {
                cout<<"\n\nGraph component "<<(i + 1)<<endl;

                cout<<"\n BFS search order :\n";
                cout<<"\n Visit order : ";

                s = dequeue(q);

                for (k = j; k < s; k++)
                    cout<<setw(2)<<trav[k]<<" ";

                cout<<"\n Distance    : ";
                for (k = j; k < s; k++)
                    cout<<setw(2)<<g->bfs_vparams[trav[k]].distance<<" ";

                cout<<endl;
                j = s;
            }
        }
    }
}

void bfs_traversal(adj_list_t *g)
{
    if (g) {
        breadth_first_search(g);
        print_bfs_paths(g);
    }
}

void bfs_parameterized(adj_list_t *g, unsigned int init_point)
{
    unsigned int s, i, j;
    int u, v;
    queue_t *q = NULL;
    queue_t *qt = NULL;
    int *trav = NULL;
    vertex_t *v_v = NULL;
    g->cnctd_cmpnts = 0;

    /*
     * Randomly pick any initial point in a graph.
     */

    s = init_point;

    for (i = 0; i < g->size; i++) {
        g->bfs_vparams[i].color = WHITE;
        g->bfs_vparams[i].distance = (~(1 << 31));
        g->bfs_vparams[i].pred_index = 1 << 31;
    }

    g->bfs_vparams[s].color = GRAY;
    g->bfs_vparams[s].distance = 0;
    g->bfs_vparams[s].pred_index = 1 << 31;

    create_queue(&q);
    enqueue(q, s);

    qt = g->bfs_params->bfs_q;
    trav = g->bfs_params->bfs_trav;
    i = 0;

    while (q->front || (i < g->size)) {
        u = dequeue(q);
        if (u > -1) {
            v_v = g->graph[u].head;
            trav[i++] = u;

            while (v_v) {
                v = v_v->val;
                if (g->bfs_vparams[v].color == WHITE) {
                    g->bfs_vparams[v].color = GRAY;
                    g->bfs_vparams[v].distance = g->bfs_vparams[u].distance + 1;
                    g->bfs_vparams[v].pred_index = u;
                    enqueue(q, v);
                }

                v_v = v_v->next;
            }
            g->bfs_vparams[u].color = BLACK;
        } else {
            for (j = 0; j < g->size; j++)
                if (g->bfs_vparams[j].color == WHITE)
                    break;

            g->bfs_vparams[j].color = GRAY;
            g->bfs_vparams[j].distance = 0;
            g->bfs_vparams[j].pred_index = 1 << 31;

            enqueue(q, j);
            enqueue(qt, i);
            g->cnctd_cmpnts++;
        }
    }

    enqueue(qt, g->size);

    qt = NULL;
    free_queue(&q);
}

void dfs_visit(adj_list_t *g, unsigned int u)
{
    unsigned int v;
    vertex_t *v_u = NULL;

    g->dfs_vparams[u].color = GRAY;
    time_count++;
    g->dfs_vparams[u].discovery_time = time_count;

    v_u = g->graph[u].head;

    while (v_u) {
        v = v_u->val;
        if (g->dfs_vparams[v].color == WHITE) {
            g->dfs_vparams[v].pred_index = u;
            dfs_visit(g, v);
        }

        v_u = v_u->next;
    }

    time_count++;
    g->dfs_vparams[u].color = BLACK;
    g->dfs_vparams[u].finish_time = time_count;
    g->dfs_params->dfs_trav[elem_count++] = u;
}

void depth_first_search(adj_list_t *g)
{
    queue_t *q = NULL;
    unsigned int i;

    g->cnctd_cmpnts = 0;
    q = g->dfs_params->dfs_q;

    for (i = 0; i < g->size; i++) {
        g->dfs_vparams[i].color = WHITE;
        g->dfs_vparams[i].pred_index = 1 << 31;
    }

    time_count = 0;
    elem_count = 0;

    for (i = 0; i < g->size; i++) {
        if (g->dfs_vparams[i].color == WHITE) {
            enqueue(q, i);
            g->cnctd_cmpnts++;
            dfs_visit(g, i);
        }
    }

    enqueue(q, g->size);
    time_count = 0;
    elem_count = 0;
}

void print_dfs_paths(adj_list_t *g)
{
    unsigned int i, j, k, s;
    queue_t *q = NULL;
    int *trav = NULL;

    q = g->dfs_params->dfs_q;
    trav = g->dfs_params->dfs_trav;

    s = dequeue(q);
    if (g) {
        if (queue_front(q) > -1) {
            j = 0;

            for (i = 1; i <= g->cnctd_cmpnts; i++) {
                cout<<"\n\nGraph component "<<i<<endl;

                cout<<"\n DFS search order  :\n";
                cout<<"\n Visit order       : ";

                s = dequeue(q);

                for (k = j; k < s; k++)
                    cout<<setw(2)<<trav[k]<<" ";

                cout<<"\n Discovery Time    : ";
                for (k = j; k < s; k++)
                    cout<<setw(2)<<g->dfs_vparams[trav[k]].discovery_time<<" ";

                cout<<endl;

                cout<<"\n Finishing Time    : ";
                for (k = j; k < s; k++)
                    cout<<setw(2)<<g->dfs_vparams[trav[k]].finish_time<<" ";

                cout<<endl;
                j = s;
            }
        }
    }
}

void dfs_traversal(adj_list_t *g)
{
    if (g) {
        depth_first_search(g);
        print_dfs_paths(g);
    }
}

void diameter(adj_list_t *g)
{
    unsigned int i = 0;
    unsigned int init_point;
    unsigned int end_point;
    unsigned int max_distance = 0;
    unsigned int temp;

    if (!g)
        return;

    breadth_first_search(g);
    if (g->cnctd_cmpnts > 0)
        return;

    for (i = 0; i < g->size; i++) {
        bfs_parameterized(g, i);
        temp = g->bfs_vparams[g->bfs_params->bfs_trav[g->size - 1]].distance;
        if (temp > max_distance) {
            max_distance = temp;
            init_point = i;
            end_point = g->bfs_params->bfs_trav[g->size - 1];
        }
    }

    cout<<"\nDiameter of Graph is : "<<max_distance<<endl;
    cout<<"\nInitial Point        : "<<init_point<<endl;
    cout<<"\nEnd Point            : "<<end_point<<endl;
}

/*
 * Graph analyser analyses the edges of a graph.
 * This analysis is done on the basis of discovering
 * time and finishing time of vertices during a depth
 * first search traversal. It classifies the edges as
 * per TREE, BACK or CROSS edges.
 */

void graph_analyzer(adj_list_t *g)
{
    unsigned int u;
    unsigned int v;
    vertex_t *v_v = NULL;
    edge_t *e = NULL;
    dfs_vparams_t *dfs_param_v = NULL;

    depth_first_search(g);
    dfs_param_v = g->dfs_vparams;
    for (u = 0; u < g->size; u++) {
        v_v = g->graph[u].head;
        e = g->edge[u].head;
        while (v_v) {
            v = v_v->val;
            if ((dfs_param_v[u].discovery_time < dfs_param_v[v].discovery_time) &&
                (dfs_param_v[v].discovery_time < dfs_param_v[v].finish_time) &&
                (dfs_param_v[v].finish_time < dfs_param_v[u].finish_time))
                e->edge_type = TREE;
            else if ((dfs_param_v[v].discovery_time < dfs_param_v[u].discovery_time) &&
                    (dfs_param_v[u].discovery_time < dfs_param_v[u].finish_time) &&
                    (dfs_param_v[u].finish_time < dfs_param_v[v].finish_time))
                    e->edge_type = BACK;
            else if ((dfs_param_v[v].discovery_time < dfs_param_v[v].finish_time) &&
                    (dfs_param_v[v].finish_time < dfs_param_v[u].discovery_time) &&
                    (dfs_param_v[u].discovery_time < dfs_param_v[u].finish_time))
                    e->edge_type = CROSS;

            v_v = v_v->next;
            e = e->next;
        }
    }
}

void swap(edge_t &a, edge_t &b)
{
    edge_t t;
    t = a;
    a = b;
    b = t;
}

int partition(edge_t *e, int p, int r)
{
    int right_most, pivot, j;

    right_most = e[r].w;
    pivot = p - 1;
    for (j = p; j < r; j++) {
        if (int(e[j].w) <= right_most) {
            pivot++;
            swap(e[pivot], e[j]);
        }
    }
    swap(e[pivot + 1], e[r]);

    return pivot + 1;
}

void quicksort(edge_t *e, int p, int r)
{
    int pivot;

    if (p < r) {
        pivot = partition(e, p, r);
        quicksort(e, p, pivot - 1);
        quicksort(e, pivot + 1, r);
    }
}

void sort_edges(adj_list_t *g, edge_t *e)
{
    unsigned int total_edges = 0;
    unsigned int i;
    unsigned int u = 0;
    edge_t *et = NULL;
    for (i = 0; i < g->size; i++)
        total_edges += g->graph[i].neighbours;

    for (i = 0; i < g->size; i++) {
        et = g->edge[i].head;
        while (et) {
            e[u].u = et->u;
            e[u].v = et->v;
            e[u].w = et->w;
            et = et->next;
            u++;
        }
    }

    //show_edge_list(g, e);
    quicksort(e, int(0), int(total_edges - 1));
    //show_edge_list(g, e);
}

adj_list_t * mst_kruskal(adj_list_t *g)
{
    unsigned int total_edges = 0;
    unsigned int i;
    adj_list_t *mst = NULL;
    bool adj_matrix_mst[NO_OF_VERTICES][NO_OF_VERTICES];
    unsigned int weight_mst[NO_OF_VERTICES][NO_OF_VERTICES];
    edge_t *e = NULL;
    edge_t *temp = NULL;
    list_edge_t *mst_e = NULL;
    universal_set_t u;

    dfs_traversal(g);
    if (g->cnctd_cmpnts > 1)
        return NULL;

    init_set(u, g);
    for (i = 0; i < g->size; i++)
        total_edges += g->graph[i].neighbours;

    e = (edge_t *)malloc(total_edges * sizeof(edge_t));
    sort_edges(g, e);

    mst_e = (list_edge_t *)malloc(sizeof(list_edge_t));
    mst_e->head = NULL;
    mst_e->tail = NULL;

    for (i = 0; i < total_edges; i++) {
        if (find_set(u, e[i].u) != find_set(u, e[i].v)) {
            temp = new edge_t;
            temp->u = e[i].u;
            temp->v = e[i].v;
            temp->w = e[i].w;
            temp->next = NULL;
            append_edge(mst_e, temp);
            temp = NULL;
            union_set(u, e[i].u, e[i].v);
        }
    }

    init_matrix(adj_matrix_mst, g->size);
    init_weight(weight_mst, g->size);
    create_matrix(adj_matrix_mst, mst_e);
    create_weight(weight_mst, mst_e);
    show_matrix(adj_matrix_mst, NO_OF_VERTICES);
    init_graph(adj_matrix_mst, weight_mst, g->size, &mst);

    return mst;
}
