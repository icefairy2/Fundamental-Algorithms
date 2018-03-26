#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <limits.h>
#include "Profiler.h"

#define new_listNode (ListNode*)malloc(sizeof(ListNode))

Profiler profiler("Profiler");

/* As it can be observed from the charts, BFS is linear
 * When varying the number of vertices, it takes more operations, because we add more edges
 * Breadth-first search explores the edges of a graph to discover every vertex that is
 * reachable from the source, thus creating a tree which defines the shortest path from the
 * source to every vertex. (path=number of edges)
 * They are good charts Brunt.
 */

enum Color { WHITE, GRAY, BLACK };

struct Node {
    int key;
    int d;
    Color color;
    Node* parent;
};

struct ListNode {
    Node* node;
    ListNode* next;
};

struct Queue {
    ListNode* front;
    ListNode* rear;
};

Node** V;
ListNode** list;
Queue queue;
long long op;

void enqueue(Node* node) {
    ListNode* temp = new_listNode;
    temp->node = node;
    temp->next = NULL;

    if (queue.front == NULL && queue.rear == NULL) {
        queue.front = queue.rear = temp;
        return;
    }
    queue.rear->next = temp;
    queue.rear = temp;
}

ListNode* dequeue() {
    ListNode* temp = queue.front;

    if (queue.front == NULL) {
        return NULL;        //queue empty
    }

    if (queue.front == queue.rear) {
        queue.front = queue.rear = NULL;
    }
    else {
        queue.front = queue.front->next;
    }
    return temp;
}

void breadth_first_search(Node* s) {
    ListNode* u;
    Node* v;
    ListNode* aux;
    int key;
    for (int i = 0; i < 200; i++) {
        op += 3;
        V[i]->d = INT_MAX;
        V[i]->color = WHITE;
        V[i]->parent = NULL;
    }

    op += 2;
    s->color = GRAY;
    s->d = 0;
    queue.front = NULL;
    queue.rear = NULL;

    op++;
    enqueue(s);
    while (queue.front != NULL) {
        op += 3;
        u = dequeue();
        key = u->node->key;
        aux = list[key];
        while (aux != NULL) {
            op += 3;
            v = aux->node;
            if (v->color == WHITE) {
                op += 4;
                v->color = GRAY;
                v->d = u->node->d + 1;
                v->parent = u->node;
                enqueue(v);
            } 
            aux = aux->next;
        }
        u->node->color = BLACK;
    }
}

void constructTest();

void prettyPrintDemo() {
    int i;
    for (i = 0; i < 8; i++) {
        if (V[i]->parent != NULL)
            printf("Node:%d, parent:%d, visited in step %d\n", V[i]->key, V[i]->parent->key, V[i]->d);
        else
            printf("Sorce node:%d, visited in step %d\n", V[i]->key, V[i]->d);
    } 
    printf("\nLevel 0:");
    for (i = 0; i < 8; i++) {
        if (V[i]->d == 0) {
            printf("   %d", V[i]->key);
            break;
        }         
    }
    printf("\nLevel 1: ");
    for (i = 0; i < 8; i++) {
        if (V[i]->d == 1) 
            printf(" %d", V[i]->key);
    }
    printf("\nLevel 2:");
    for (i = 0; i < 8; i++) {
        if (V[i]->d == 2) 
            printf(" %d", V[i]->key);
    }
    printf("\nLevel 3:  ");
    for (i = 0; i < 8; i++) {
        if (V[i]->d == 3) 
            printf(" %d", V[i]->key);
    }
    printf("\n");
}

//resets the first size nr nodes in the adjacency list
void freeListChains(int size) {
    ListNode* auxP;
    ListNode* aux;
    for (int i = 0; i < size; i++) {
        if (list[i] == NULL)
            continue;
        aux = list[i];
        while (aux != NULL) {
            auxP = aux;
            aux = aux->next;
            free(auxP);
        }
        list[i] = NULL;
    }
}

//checks if edge is in the adjacency list
int checkEdge(int x, int y) {
    ListNode* temp = list[x];
    while (temp != NULL) {
        if (temp->node->key == y)
            return 0;
        temp = temp->next;
    }
    return 1;
}

int main() {
    srand(time(NULL));
    V = (Node**)malloc(200 * sizeof(Node*));
    list = (ListNode**)malloc(200 * sizeof(ListNode*));

    int i;
    op = 0;

    for (i = 0; i < 200; i++)
    {
        V[i] = (Node*)malloc(sizeof(Node));
        V[i]->key = i;
        list[i] = NULL;
    }

    constructTest();
    Node* source = V[1];
    breadth_first_search(source);
    prettyPrintDemo();
    freeListChains(8);

    int n;
    int x, y;
    int ok;
    ListNode* p;
    ListNode* temp;

    //|V|=100, E variable
    for (n = 1000; n <= 5000; n += 100) {
        printf("v=100, e=%d\n", n);

        x = rand() % 100;
        source = V[x];

        //add a chain from the source of at least 100 edges
        for (i = 0; i < 100; i++) {
            y = rand() % 100;
            ok = 0;

            //add directed edge
            while (ok == 0) {
                while (x == y || checkEdge(x, y) == 0) {
                    x = rand() % 100;
                    y = rand() % 100;
                }

                p = new_listNode;
                p->node = V[y];
                p->next = NULL;
                if (list[x] == NULL)
                    list[x] = p;
                else {
                    temp = list[x];
                    while (temp->next != NULL) {
                        temp = temp->next;
                    }
                    temp->next = p;
                }

                x = y;
                ok = 1;               
            }
        }

        //add edges randomly
        for (i = 100; i < n; i++) {
            x = rand() % 100;
            y = rand() % 100;
            ok = 0;

            //add directed edge
            while (ok == 0) {
                while (x == y || checkEdge(x, y) == 0) {
                    x = rand() % 100;
                    y = rand() % 100;
                }

                p = new_listNode;
                p->node = V[y];
                p->next = NULL;
                if (list[x] == NULL)
                    list[x] = p;
                else {
                    temp = list[x];
                    while (temp->next != NULL) {
                        temp = temp->next;
                    }
                    temp->next = p;
                }

                ok = 1;
            }
        }

        op = 0;
        breadth_first_search(source);
        profiler.countOperation("Breadth-First |V|=100 ", n, op);
        freeListChains(100);
    }

    //|E|=9000, V variable
    for (n = 100; n <= 200; n += 10) {
        printf("v=%d, e=9000\n", n);

        x = rand() % n;
        source = V[x];

        //add the rest of the edges randomly
        for (i = 0; i < 9000; i++) {
            x = rand() % n;
            y = rand() % n;
            ok = 0;

            //add directed edge
            while (ok == 0) {
                while (x == y || checkEdge(x, y) == 0) {
                    x = rand() % n;
                    y = rand() % n;
                }

                p = new_listNode;
                p->node = V[y];
                p->next = NULL;
                if (list[x] == NULL)
                    list[x] = p;
                else {
                    temp = list[x];
                    while (temp->next != NULL) {
                        temp = temp->next;
                    }
                    temp->next = p;
                }

                ok = 1;
            }
        }

        op = 0;
        breadth_first_search(source);
        profiler.countOperation("Breadth-First |E|=9000 ", n, op);
        freeListChains(n);
    }

    profiler.showReport();
    _getch;
    return 0;
}







void constructTest() {
    //Hardcoded graph, textbook pg 596
    //source = list[1] (V[1]) s
    //size 8

    ListNode* aux;
    ListNode* p;

    //node r
    p = new_listNode;
    p->node = V[1];
    list[0] = p;
    aux = p;
    p = new_listNode;
    p->node = V[4];
    aux->next = p;
    p->next = NULL;

    //node s
    p = new_listNode;
    p->node = V[0];
    list[1] = p;
    aux = p;
    p = new_listNode;
    p->node = V[5];
    aux->next = p;
    p->next = NULL;

    //node t
    p = new_listNode;
    p->node = V[3];
    list[2] = p;
    aux = p;
    p = new_listNode;
    p->node = V[5];
    aux->next = p;
    aux = p;
    p = new_listNode;
    p->node = V[6];
    aux->next = p;
    p->next = NULL;

    //node u 
    p = new_listNode;
    p->node = V[2];
    list[3] = p;
    aux = p;
    p = new_listNode;
    p->node = V[6];
    aux->next = p;
    aux = p;
    p = new_listNode;
    p->node = V[7];
    aux->next = p;
    p->next = NULL;

    //node v
    p = new_listNode;
    p->node = V[0];
    list[4] = p;
    p->next = NULL;

    //node w 
    p = new_listNode;
    p->node = V[1];
    list[5] = p;
    aux = p;
    p = new_listNode;
    p->node = V[2];
    aux->next = p;
    aux = p;
    p = new_listNode;
    p->node = V[6];
    aux->next = p;
    p->next = NULL;

    //node x
    p = new_listNode;
    p->node = V[2];
    list[6] = p;
    aux = p;
    p = new_listNode;
    p->node = V[3];
    aux->next = p;
    aux = p;
    p = new_listNode;
    p->node = V[5];
    aux->next = p;
    aux = p;
    p = new_listNode;
    p->node = V[7];
    aux->next = p;
    p->next = NULL;

    //node y
    p = new_listNode;
    p->node = V[3];
    list[7] = p;
    aux = p;
    p = new_listNode;
    p->node = V[6];
    aux->next = p;
    p->next = NULL;
}