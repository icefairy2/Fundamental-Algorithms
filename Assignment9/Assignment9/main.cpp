/*
* The charts correspond to the formula: O(E + V). this is visible mostly on the first chart, since the variation is the biggest there.
* Counter intuitively the small increase of the number of vertices did not have a huge impact on the number of operations. (second graph)
*/
#include <stdio.h>
#include "Profiler.h"

Profiler profiler;
int nr_operations;

typedef enum ColorsTag {
    WHITE, GRAY, BLACK
}ColorsT;

typedef struct NodeTag {
    struct NodeTag *next;
    int value;
}NodeT;

typedef struct LinkedListTag {
    NodeT *first;
    NodeT *last;
}LinkedListT;

typedef struct GraphTag {
    int nr_vertices;
    LinkedListT** adjLists;

    int* vertices_parent;
    int *colors;
    int *distance;
}GraphT;

NodeT* create_NodeT(int value)
{
    NodeT *node = (NodeT*)malloc(sizeof(NodeT));
    if (node == NULL) {
        printf("memory exhaustion\n");
        exit(1);
    }

    node->next = NULL;
    node->value = value;

    return node;
}

GraphT* create_GraphT(int nr_vertices, LinkedListT** adjLists)
{
    GraphT* graph = (GraphT*)malloc(sizeof(GraphT));
    if (graph == NULL) {
        printf("memory exhasustion");
        exit(1);
    }

    graph->vertices_parent = (int*)malloc(sizeof(int) * nr_vertices);
    if (graph->vertices_parent == NULL) {
        printf("memory exhasustion");
        exit(1);
    }

    graph->colors = (int*)malloc(sizeof(int) * nr_vertices);
    if (graph->colors == NULL) {
        printf("memory exhasustion");
        exit(1);
    }

    graph->distance = (int*)malloc(sizeof(int) * nr_vertices);
    if (graph->distance == NULL) {
        printf("memory exhasustion");
        exit(1);
    }

    graph->nr_vertices = nr_vertices;
    graph->adjLists = adjLists;

    return graph;
}

LinkedListT* create_LinkedListT()
{
    LinkedListT* list = (LinkedListT*)malloc(sizeof(LinkedListT));
    if (list == NULL) {
        printf("memory exhaustion\n");
        exit(1);
    }

    list->first = NULL;
    list->last = NULL;

    return list;
}

void add_to_end_of_list(LinkedListT* list, int value)
{
    NodeT* node = create_NodeT(value);
    if (list->first == NULL) {
        list->first = node;
        list->last = node;
        return;
    }

    list->last->next = node;
    list->last = node;
}

int remove_from_beg_of_list(LinkedListT* list)
{
    if (list->first == NULL) {
        return -1;
    }

    int ret = list->first->value;
    list->first = list->first->next;

    return ret;
}

int find_index_of_root_ParentRepT(int* parentRep, int size)
{
    for (int i = 0; i < size; i++) {
        if (parentRep[i] == -1) {
            return i;
        }
    }

    return -1;
}

void pretty_print_ParentRepT_rec(int* parentRep, int size, int root_index, int level)
{
    for (int i = 0; i < level; i++) {
        printf("   ");
    }
    printf("%d\n", root_index);
    for (int i = 0; i < size; i++) {
        if (parentRep[i] == root_index) {
            pretty_print_ParentRepT_rec(parentRep, size, i, level + 1);
        }
    }
}

void pretty_print_ParentRepT(int* parentRep, int size)
{
    int root_index = find_index_of_root_ParentRepT(parentRep, size);
    pretty_print_ParentRepT_rec(parentRep, size, root_index, 0);
}

void depth_first_search(GraphT* graph, int source)
{
    nr_operations = 0;
    for (int i = 0; i < graph->nr_vertices; i++) {
        nr_operations += 3;
        graph->colors[i] = WHITE;
        graph->distance[i] = INT_MAX;
        graph->vertices_parent[i] = -1;
    }

    nr_operations += 3;
    graph->colors[source] = GRAY;
    graph->distance[source] = 0;
    graph->vertices_parent[source] = -1;

    LinkedListT* queue = create_LinkedListT();
    add_to_end_of_list(queue, source);
    nr_operations++;

    while (queue->first != NULL) {
        nr_operations += 3;
        int u = remove_from_beg_of_list(queue);
        LinkedListT* adjList = graph->adjLists[u];
        NodeT* walker = adjList->first;
        while (walker != NULL) {
            nr_operations += 3;
            int v = walker->value;
            if (graph->colors[v] == WHITE) {
                nr_operations += 4;
                graph->colors[v] = GRAY;
                graph->distance[v] = graph->distance[u] + 1;
                graph->vertices_parent[v] = u;
                add_to_end_of_list(queue, v);
            }

            walker = walker->next;
        }
        graph->colors[u] = BLACK;
    }
}

GraphT* generate_graph(int v, int e)
{
    int adjacent_vertices[1000];

    LinkedListT** adjLists = (LinkedListT**)malloc(sizeof(LinkedListT*) * v);
    if (adjLists == NULL) {
        return NULL;
    }

    for (int j = 0; j < v; j++) {
        //for each vertex
        adjLists[j] = create_LinkedListT();
        FillRandomArray(adjacent_vertices, e / v, 0, v, true);

        for (int k = 0; k < e / v; k++) {
            add_to_end_of_list(adjLists[j], adjacent_vertices[k]);
        }
    }

    return create_GraphT(v, adjLists);
}

int main()
{
    LinkedListT* adjLists[8];

    for (int i = 0; i < 8; i++) {
        adjLists[i] = create_LinkedListT();
    }

    add_to_end_of_list(adjLists[0], 1);
    add_to_end_of_list(adjLists[0], 7);

    add_to_end_of_list(adjLists[1], 0);
    add_to_end_of_list(adjLists[1], 6);

    add_to_end_of_list(adjLists[2], 3);
    add_to_end_of_list(adjLists[2], 5);
    add_to_end_of_list(adjLists[2], 6);

    add_to_end_of_list(adjLists[3], 2);
    add_to_end_of_list(adjLists[3], 4);
    add_to_end_of_list(adjLists[3], 5);

    add_to_end_of_list(adjLists[4], 3);
    add_to_end_of_list(adjLists[4], 5);

    add_to_end_of_list(adjLists[5], 2);
    add_to_end_of_list(adjLists[5], 3);
    add_to_end_of_list(adjLists[5], 4);
    add_to_end_of_list(adjLists[5], 6);

    add_to_end_of_list(adjLists[6], 1);
    add_to_end_of_list(adjLists[6], 2);
    add_to_end_of_list(adjLists[6], 5);

    add_to_end_of_list(adjLists[7], 0);

    GraphT* graph_demo = create_GraphT(8, adjLists);
    depth_first_search(graph_demo, 1);

    pretty_print_ParentRepT(graph_demo->vertices_parent, graph_demo->nr_vertices);
    system("pause");

    int v = 100;
    for (int i = 10; i < 50; i++) {
        int e = i * 100;

        GraphT* graph = generate_graph(v, e);
        depth_first_search(graph, 0);
        profiler.countOperation("nr operations for increasing E", e, nr_operations);
    }

    int e = 9000;
    for (int i = 10; i < 20; i++) {
        v = i * 10;

        GraphT* graph = generate_graph(v, e);
        depth_first_search(graph, 0);
        profiler.countOperation("nr operations for increasing V", v, nr_operations);
    }

    profiler.showReport();

    return 0;
}