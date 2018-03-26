#include <stdio.h>
#include <stdlib.h>
#include<conio.h>
#include "Profiler.h"

#define MAX_SIZE 10000

/* The algorithm takes O(nlogk) time so the chart where the n is varied 
 * and the k is fixed show linearity, and of course, the bigger the k, 
 * the more operations.
 * When fixing n and varying k, as the chart shows, we get a logarothmic curve
 * as we should, according to the running time
 */


Profiler profiler("Profiler");

struct Node {
    int key;
    Node* next;
};

struct List {
    int size;
    Node* first;
    Node* last;
};

int a[MAX_SIZE];
int heapSize;
int op;

void swap(Node** x, Node** y) {
    Node* z = *x;
    *x = *y;
    *y = z;
}

void minHeapify(Node* arr[], int i) {
    int l = 2 * i;
    int r = 2 * i + 1;
    int smallest;

    op++;
    if (l <= heapSize && arr[l]->key < arr[i]->key) {
        smallest = l;
    }
    else {
        smallest = i;
    }

    op++;
    if (r <= heapSize && arr[r]->key < arr[smallest]->key) {
        smallest = r;
    }

    if (smallest != i) {
        op += 3;
        swap(&arr[i], &arr[smallest]);
        minHeapify(arr, smallest);
    }
}

void buildMinHeap(Node* arr[], int n) {
    heapSize = n;
    for (int i = n / 2; i > 0; i--) {
        minHeapify(arr, i);
    }
}

void insertFirst(List* list, int newKey) {
    Node* n = (Node*)(malloc(sizeof(Node)));
    n->key = newKey;
    n->next = NULL;

    //list empty
    if (list->first == NULL) {
        list->first = n;
        list->last = n;
        list->size = 1;
    }
    else {
        n->next = list->first;
        list->first = n;
        list->size++;
    }
}

void printList(List* list) {
    Node *n = list->first;
    printf("List: ");
    while (n != NULL) {
        printf("%d ", n->key);
        n = n->next;
    }
    printf("\n");
}

void printHeap(Node* heap[], int k) {
    int i;
    printf("Heap: ");
    for (i = 1; i <= k; i++) {
        printf("%d ", heap[i]->key);
    }
    printf("\n");
}

List* generate(int k, int n) {
    int i;
    
    List* listArr = (List*)malloc(k * sizeof(List));
    for (i = 0; i < k; i++)
        listArr[i].first = listArr[i].last = NULL;

    //We generate an inversely sorted array 
    FillRandomArray(a, n, 1, 10000, false, 2);

    //make sure each list holds at least one element
    for (i = 0; i < k; i++)
        insertFirst(&listArr[i], a[i]);

    int random;
    for (i = k; i < n; i++) {
        random = rand() % k;
        insertFirst(&listArr[random], a[i]);
    }
    return listArr;
}

int* mergeK(List* listArr, int k, int n) {
    int i;

    for (i = 0; i < k; i++)
        printList(&listArr[i]);

    printf("\n");
    
    Node** heap = (Node**)malloc((k + 1) * sizeof(Node*));

    for (i = 0; i < k; i++) {
        op++;
        heap[i + 1] = listArr[i].first;
    }

    int* finalArr = (int*)malloc(n * sizeof(int));
    i = 0;
    heapSize = k;

    buildMinHeap(heap, heapSize);

    while (heap[1] != NULL) {
        printHeap(heap, heapSize);
        finalArr[i] = heap[1]->key;
        i++;
        heap[1] = heap[1]->next;
        op += 2;

        if (heap[1] == NULL) {
            int j;
            for (j = 1; j < heapSize; j++) {
                op++;
                heap[j] = heap[j + 1];
            }
            heapSize--;
            if ((heapSize >= 2) && (heap[1]->key > heap[2]->key)) {
                op += 4;
                swap(&heap[1], &heap[2]);
            }
            if ((heapSize >= 2) && (heap[1]->key > heap[heapSize]->key)) {
                op += 4;
                swap(&heap[1], &heap[heapSize]);
            }
        }
        else {
            minHeapify(heap, 1);
        }
    }
    return finalArr;
}

int main() {
    

    int nIt, kIt;
    int i, k, n;
    printf("k = ");
    scanf_s("%d", &k);

    printf("n = ");
    scanf_s("%d", &n);
    
    /*List* listArr = (List*)malloc(1000 * sizeof(List));
    int* finalArr = (int*)malloc(MAX_SIZE * sizeof(int));*/
    
    List* listArr = generate(k, n);
    int* finalArr = mergeK(listArr, k, n);

    printf("\nFinal list: ");
    for (i = 0; i < n; i++) {
        printf("%d ", finalArr[i]);
    }
    printf("\nIs it sorted? %s\n", (IsSorted(finalArr, n) ? "yes" : "no"));

    //Create reports
   /* int k1 = 5, k2 = 10, k3 = 100;
    for (nIt = 100; nIt <= 10000; nIt += 100) {
        op = 0;
        listArr = generate(k1, nIt);
        finalArr = mergeK(listArr, k1, nIt);
        profiler.countOperation("Merge k=5", nIt, op);
        op = 0;
        listArr = generate(k2, nIt);
        finalArr = mergeK(listArr, k2, nIt);
        profiler.countOperation("Merge k=10", nIt, op);
        op = 0;
        listArr = generate(k3, nIt);
        finalArr = mergeK(listArr, k3, nIt);
        profiler.countOperation("Merge k=100", nIt, op);
    }

    nIt = MAX_SIZE;
    for (kIt = 10; kIt <= 500; kIt += 10) {
        op = 0;
        listArr = generate(kIt, nIt);
        finalArr = mergeK(listArr, kIt, nIt);
        profiler.countOperation("Merge n=10000", kIt, op);
    }


    profiler.createGroup("First case", "Merge k=5", "Merge k=10", "Merge k=100");
    profiler.showReport();*/

    _getch();
    return 0;
}