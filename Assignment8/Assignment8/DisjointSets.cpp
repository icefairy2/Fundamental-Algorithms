#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include "Profiler.h"

Profiler profiler("Profiler");

struct NodeDS {
    int key;
    int rank;
    NodeDS* parent;
};

struct Edge {
    int u;
    int v;
    int weight;
};

NodeDS** nodes;
Edge** G;
Edge** A;
int size;
int op;
Edge* aux;

void makeSet(int x, NodeDS* node) {
    node->key = x;
    node->rank = 1;
    node->parent = node;
}

NodeDS* findSet(NodeDS* n) {
    op++;
    if (n != n->parent) {
        op++;
        n->parent = findSet(n->parent);
    }
    return n->parent;
}

void link(NodeDS* x, NodeDS* y) {
    if (x->rank > y->rank) {
        y->parent = x;
        op++;
    }
       
    else {
        op++;
        x->parent = y;
        if (x->rank == y->rank)
            y->rank++;
    }
}

void unionSet(NodeDS* x, NodeDS* y) {
    link(findSet(x), findSet(y));
}

Edge** MstKruskal(Edge* G[], int n, int nrEdges) {

    int i;
    size = 0;

    for (i = 0; i < n; i++) {
        op++;
        makeSet(i, nodes[i]);
    }
    for (i = 0; i < nrEdges - 1; i++) {
        for (int j = i; j < nrEdges; j++) {
            if (G[i]->weight > G[j]->weight) {
                op += 3;
                aux->u = G[i]->u;
                aux->v = G[i]->v;
                aux->weight = G[i]->weight;
                G[i]->u = G[j]->u;
                G[i]->v = G[j]->v;
                G[i]->weight = G[j]->weight;
                G[j]->u = aux->u;
                G[j]->v = aux->v;
                G[j]->weight = aux->weight;
            }
        }
    }

    for (i = 0; i < nrEdges; i++) {
        if (findSet(nodes[G[i]->u]) != findSet(nodes[G[i]->v])) {
            op++;
            A[size] = G[i];
            size++;
            unionSet(nodes[G[i]->u], nodes[G[i]->v]);
        }
    }

    return A;
}

int main() {
    srand(time(NULL));
    int i;
    aux = (Edge*)malloc(sizeof(Edge));
    nodes = (NodeDS**)malloc(10000 * sizeof(NodeDS*));
    for (i = 0; i < 10000; i++)
    {
        nodes[i] = (NodeDS*)malloc(sizeof(NodeDS));
    }

    G = (Edge**)malloc(40000 * sizeof(Edge*));
    for (i = 0; i < 40000; i++)
        G[i] = (Edge*)malloc(sizeof(Edge));

    A = (Edge**)malloc(40000 * sizeof(Edge*));
    for (i = 0; i < 40000; i++)
        A[i] = (Edge*)malloc(sizeof(Edge));

    for (i = 0; i < 10; i++) {
        makeSet(i, nodes[i]);
    }

    for (i = 0; i < 5; i++) {
    int u = rand() % 10;
    int v = rand() % 10;
    if (u == v)
    v = rand() % 10;
    printf("Edge: (%d, %d)\n", u, v);
    printf("Before union: findSet(%d) = %d and findSet(%d) = %d\n", nodes[u]->key, findSet(nodes[u])->key, nodes[v]->key, findSet(nodes[v])->key);
    unionSet(nodes[u], nodes[v]);
    printf("After union: findSet(%d) = %d and findSet(%d) = %d\n", nodes[u]->key, findSet(nodes[u])->key, nodes[v]->key, findSet(nodes[v])->key);
    }

    G[0]->u = 0;
    G[0]->v = 1;
    G[0]->weight = 3;

    G[1]->u = 0;
    G[1]->v = 3;
    G[1]->weight = 1;

    G[2]->u = 0;
    G[2]->v = 2;
    G[2]->weight = 2;

    G[3]->u = 1;
    G[3]->v = 3;
    G[3]->weight = 4;

    G[4]->u = 2;
    G[4]->v = 3;
    G[4]->weight = 3;

    A = MstKruskal(G, 4, 5);
    printf("\nMinimum spanning tree:\n");
    for (i = 0; i < size; i++)
    printf("(%d, %d)=%d\n", A[i]->u, A[i]->v, A[i]->weight);

   /* for (int n = 100; n <= 10000; n += 100) {
        printf("%d\n", n);
        for (i = 0; i < 4 * n; i++) {
            G[i]->u = rand() % n;
            G[i]->v = rand() % n;
            G[i]->weight = rand() % n;
        }
        op = 0;
        A = MstKruskal(G, n, 4 * n);
        profiler.countOperation("Kruskal operations", n, op);
        profiler.countOperation("MST size", n, size);
    }*/

    for (i = 0; i < 40000; i++) {
        //free(A[i]);
        free(G[i]);
    }
    for (i = 0; i < 10000; i++)
        free(nodes[i]);
    free(aux);
   // profiler.showReport();
    _getch;
    return 0;
}