#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <limits.h>
#include "Profiler.h"

#define new_listNode (ListNode*)malloc(sizeof(ListNode))

Profiler profiler("Profiler");

/* As it can be observed from the charts, DFS is linear, it reaches all vertices
 * It creates strongly connected components
 * 
 * Remains of almost implemented Tarjan algorithm commented within the code
 */

enum Color { WHITE, GRAY, BLACK };

struct Node {
    int key;
    int d;
    int f;
    //int index;
    //int lowLink;
    //bool onStack;
    Color color;
    Node* parent;
};

struct ListNode {
    Node* node;
    ListNode* next;
};

struct LinkedList {
    ListNode* first;
};

Node** V;
ListNode** list;
LinkedList Llist;
long long op;
int timeStamp;
//int index;
//int sizeSCC;

/*void push(Node* node) {
    ListNode* temp = new_listNode;
    temp->node = node;
    temp->next = NULL;

    if (stack.top == NULL) {
        stack.top = temp;
        return;
    }
    temp->next = stack.top;
    stack.top = temp;
}

ListNode* pop() {
    ListNode* temp = stack.top;

    if (stack.top == NULL) {
        return NULL;        //stack empty
    }

    if (stack.top->next == NULL) {
        stack.top = NULL;
    }
    else {
        stack.top = stack.top->next;
    }
    return temp;
}*/

void dfs_visit(Node* node) {
    timeStamp++;
    node->d = timeStamp;
    node->color = GRAY;
    op += 2;
    ListNode* aux = list[node->key];
    Node* v;
    while (aux != NULL) {
        op += 3;
        v = aux->node;
        if (v->color == WHITE) {
            op ++;
            v->parent = node;
            dfs_visit(v);
        }
        aux = aux->next;
    }
    op += 3;
    node->color = BLACK;
    timeStamp++;
    node->f = timeStamp;
    
}

void depth_first_search(int size) {
    int i;
    for (i = 0; i < size; i++) {
        op += 2;
        V[i]->color = WHITE;
        V[i]->parent = NULL;
    }
    timeStamp = 0;
    for (i = 0; i < size; i++) {
        op++;
        if (V[i]->color == WHITE)
            dfs_visit(V[i]);
    }
}

void dfs_visit_sort(Node* node) {
    timeStamp++;
    node->d = timeStamp;
    node->color = GRAY;

    ListNode* aux = list[node->key];
    Node* v;
    while (aux != NULL) {
        v = aux->node;
        if (v->color == WHITE) {
            op++;
            v->parent = node;
            dfs_visit_sort(v);
        }
        aux = aux->next;
    }
    node->color = BLACK;
    timeStamp++;
    node->f = timeStamp;

    ListNode* n = new_listNode;
    n->node = node;
    if (Llist.first == NULL) {
        Llist.first = n;
        n->next = NULL;
    }
    else {
        n->next = Llist.first;
        Llist.first = n;
    }
}

void topological_sort(int size) {
    int i;
    ListNode* node;
    for (i = 0; i < size; i++) {
        V[i]->color = WHITE;
        V[i]->parent = NULL;
    }
    timeStamp = 0;
    for (i = 0; i < size; i++) {
        if (V[i]->color == WHITE) {
            dfs_visit_sort(V[i]);
        }
    }

}

void printSort(ListNode* Lnode) {
    if (Lnode != NULL) {
        printSort(Lnode->next);
        printf("Node:%d, finishing time:%d\n", Lnode->node->key, Lnode->node->f);
    }
}

/*ListNode* strongconnect(Node* v) {
    ListNode* scc;
    v->index = index;
    v->lowlink = index;

    index++;
    push(v);
    v->onStack = true;

    ListNode* aux = list[v->key];
    Node* w;
    while (aux != NULL) {
        w = aux->node;
        if (w->index == INT_MAX) {
            scc = strongconnect(w);
            v->lowlink = min(v->lowlink, w->lowlink);
        }
        aux = aux->next;
    }

    if (v->lowlink == v->index) {
        scc = new_listNode;
        ListNode* y;

        scc->next = NULL;
        scc = y = pop();
        scc->node->onStack = false;

        aux = scc;

        while (y->node == v) {
            y = pop();
            y->node->onStack = false;
            aux->next = y;
            aux = aux->next;
        } //while (y->node != v);
    }
    return scc;
}

ListNode** tarjan(int size) {
    index = 0;
    ListNode** tarj;
    int i;
    sizeSCC = 0;
    for (i = 0; i < size; i++) {
        if (V[i]->index == INT_MAX) {
            tarj[sizeSCC] = strongconnect(V[i]);
            sizeSCC++;
        }
    }
    return tarj;
}*/

void constructTest();

void printGraph() {
    int i;
    ListNode* aux;
    for (i = 0; i < 6; i++) {
        if (list[i] != NULL) {
            printf("Node: %d, has outgoing edge pointing towards: ", V[i]->key);
            aux = list[i];
            while (aux != NULL) {
                printf("%d. ", aux->node->key);
                aux = aux->next;
            }
            printf("\n");
        }
        else
            printf("Node: %d, has no outgoing edges", V[i]->key);

    }
}

/*void printSCC() {
    int i;
    ListNode* aux;
    ListNode** tarj = tarjan(6);
    for (i = 0; i < sizeSCC; i++) {
        if (tarj[i] != NULL) {
            printf("Component: %d, contains vertices: ", tarj[i]->node->key);
            aux = tarj[i];
            while (aux != NULL) {
                printf("%d. ", aux->node->key);
                aux = aux->next;
            }
            printf("\n");
        }
        else
            printf("Component not found", V[i]->key);

    }
}*/

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
    printf("Graph:\n");
    printGraph();
    depth_first_search(6);
    topological_sort(6);
    ListNode* Lnode = Llist.first;
    printf("\nTopological sort:\n");
    printSort(Lnode);
    //printSCC();

    int n;
    int x, y;
    int ok;
    ListNode* p;
    ListNode* temp;

    //|V|=100, E variable
    for (n = 1000; n <= 5000; n += 100) {
        printf("v=100, e=%d\n", n);

        //add edges randomly
        for (i = 0; i < n; i++) {
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
        depth_first_search(100);
        profiler.countOperation("Depth-First |V|=100 ", n, op);
        freeListChains(100);
    }

    //|E|=9000, V variable
    for (n = 100; n <= 200; n += 10) {
        printf("v=%d, e=9000\n", n);

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
        depth_first_search(n);
        profiler.countOperation("Depth-First |E|=9000 ", n, op);
        freeListChains(n);
    }

    profiler.showReport();
    _getch;
    return 0;
}



void constructTest() {
    //Hardcoded graph, textbook pg 605
    //source = list[0] (V[0]) u
    //size 6

    ListNode* aux;
    ListNode* p;

    //node u
    p = new_listNode;
    p->node = V[1];
    list[0] = p;
    aux = p;
    p = new_listNode;
    p->node = V[3];
    aux->next = p;
    p->next = NULL;

    //node v
    p = new_listNode;
    p->node = V[4];
    list[1] = p;
    p->next = NULL;

    //node w
    p = new_listNode;
    p->node = V[4];
    list[2] = p;
    aux = p;
    p = new_listNode;
    p->node = V[5];
    aux->next = p;
    p->next = NULL;

    //node x 
    p = new_listNode;
    p->node = V[1];
    list[3] = p;
    p->next = NULL;

    //node y
    p = new_listNode;
    p->node = V[3];
    list[4] = p;
    p->next = NULL;

    //node z 
    p = new_listNode;
    p->node = V[5];
    list[5] = p;
    p->next = NULL;
}



