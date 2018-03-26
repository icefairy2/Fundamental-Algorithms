#include <stdio.h>
#include<conio.h>
#include<array>
#include "Profiler.h"

#define MAX_SIZE 10000

/*
 * Bottom-up build heap approach runs with O(n) complexity, while
 * Top-down build heap approach runs with O(n*logn) complexity
 * The linearity is clearly observable from the charts
 * Bottom-up approach has less operations (=> faster) but the array size is fixed
 * Top-down strategy is slower, has more operations, but a new element can be inserted any time,
 * so data size is dynamic
 *
 * Worst case: when we build a heap from an already sorted array (max heap)
 * Top-down procedure here has significantly more operations because of the new element assignments
 * (bottom-up: we work on an array; top-down: a new element has to be inserted first)
 * Bottom-up approach does two comparisons and a swap at every step =>5 operations/step
 * in the worst case all these are executed => total operation numbers are multiples of 5
 */


int heap_size;

Profiler profiler("Heaps");

//Procedure swaps the values at x and y
void swap(int &x, int &y) {
    int z = x;
    x = y;
    y = z;
}

//Procedure copies an array a into an array b of length n both
void copy(int a[], int b[], int n) {
    for (int i = 0; i < n; i++)
        b[i] = a[i];
}

/****************************************
 * Procedures for the bottom-up approach
 ****************************************/
int maxHeapify(int arr[], int i) {
    int l = 2 * i + 1;
    int r = 2 * i + 2;
    int largest;
    int op = 0;

    op++;
    if (l <= heap_size && arr[l] > arr[i]) {
        largest = l;
    }
    else {
        largest = i;
    }

    op++;
    if (r <= heap_size && arr[r] > arr[largest]) {
        largest = r;
    }

    if (largest != i) {
        op += 3;
        swap(arr[i], arr[largest]);
        return op + maxHeapify(arr, largest);
    }
    return op;
}

int buildMaxHeapBU(int arr[], int n) {
    int opn = 0;
    heap_size = n - 1;
    for (int i = n / 2 - 1; i >= 0; i--) {
        opn += maxHeapify(arr, i);
    }
    return opn;
}

/****************************************
* Procedures for the top-down approach
****************************************/
int heapIncreaseKey(int arr[], int i, int key) {
    int op = 0;
    if (key < arr[i]) {
        return 0;
    }
    op++;
    arr[i] = key;
    while (i > 0 && arr[i / 2] < arr[i]) {
        op += 4;
        swap(arr[i], arr[i / 2]);
        i /= 2;
    }
    if (i != 0)
        op++;
    return op;
}

int maxHeapInsert(int arr[], int key) {
    heap_size++;
    arr[heap_size] = INT_MIN;
    return heapIncreaseKey(arr, heap_size, key);
}

int buildMaxHeapTD(int arr[], int n) {
    int opn = 0;
    heap_size = 0;
    for (int i = 1; i < n; i++) {
        opn += maxHeapInsert(arr, arr[i]);
    }
    return opn;
}

//Heapsort algorithm
void heapSort(int arr[], int n) {
    buildMaxHeapBU(arr, n);
    for (int i = n - 1; i > 0; i--) {
        swap(arr[0], arr[i]);
        heap_size--;
        maxHeapify(arr, 0);
    }
}

void testRun() {

    int tst[16] = { 23, 2, 11, 5856, 2, 2, 121, 10, 30, 40, 50, 1, 1, 421, 33 };
    int n = 15;		//length of array, set it correspondingly

                    //uncomment next line if wanted to generate random array
    FillRandomArray(tst, n, 1, 1000);

    //prints initial array
    printf("Initial array:\n");
    for (int i = 0; i < n; i++) {
        printf("%d ", tst[i]);
    }
    printf("\nIs it sorted? %s", (IsSorted(tst, n) ? "yes" : "no"));  //these lines verify after each case if the array is sorted

                                                                      //sorts in bubblesort then prints array
    heapSort(tst, n);
    printf("\n\nHeap sorted array:\n");
    for (int i = 0; i < n; i++) {
        printf("%d ", tst[i]);
    }
    printf("\nIs it sorted? %s\n", (IsSorted(tst, n) ? "yes" : "no"));
}

int main() {
    int a[MAX_SIZE];
    int b[MAX_SIZE];
    int n;

    int opb, opt, j, op;

    testRun();

    for (n = 100; n < MAX_SIZE; n += 100) {

        opb = 0;
        opt = 0;

        for (j = 0; j < 5; j++) {
            FillRandomArray(a, n, 1, 100000, false, 0);
            copy(a, b, n);
            opb += buildMaxHeapBU(b, n);
            copy(a, b, n);
            opt += buildMaxHeapTD(b, n);
        }
        profiler.countOperation("BottomUp operations", n, opb / 5);
        profiler.countOperation("TopDown operations", n, opt / 5);

        //Worst case: array is unique and sorted
        FillRandomArray(a, n, 1, 100000, true, 1);
        copy(a, b, n);
        op = buildMaxHeapBU(b, n);
        profiler.countOperation("BottomUp worst", n, op);
        copy(a, b, n);
        op = buildMaxHeapTD(b, n);
        profiler.countOperation("TopDown worst", n, op);
    }

    profiler.createGroup("Total average", "BottomUp operations", "TopDown operations");
    profiler.createGroup("Worst case", "BottomUp worst", "TopDown worst");

    profiler.showReport();
    //_getch();
    return 0;
}