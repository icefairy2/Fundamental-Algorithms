#include <stdio.h>
#include <conio.h>
#include "Profiler.h"

#define MAX_SIZE 10000

/*
* As one would expect, quicksort runs better in the average case, than
* heapsort. Quicksort's complexity is O(nlogn) which is equal to its
* best case performance. Even though heapsort's complexity is also O(nlogn),
* quicksort is faster.
* We can observe how efficient quicksort is, from the next two charts, because
* in the average case it runs with a number of operations very close to the best.
* We get the best case when the partitions are always mostly equal (the array and 
* subarrays are split in half), and worst case can only result from a totally 
* sorted or reversely sorted array, when the partitions are 0 and the rest.
* The difference between the number of operations in these extreme cases
* is significantly large.
* At quicksort in the worst case we can observe almost exponential growth, 
* while in the average and best cases this algorithm shows linearity.
*/

int heap_size;
int opq, oph;

Profiler profiler("Heaps");

void swap(int &x, int &y) {
    int z = x;
    x = y;
    y = z;
}

void copy(int a[], int b[], int n) {
    for (int i = 0; i < n; i++)
        b[i] = a[i];
}

//Heapsort algorithm
void maxHeapify(int arr[], int i) {
    int l = 2 * i + 1;
    int r = 2 * i + 2;
    int largest;

    oph++;
    if (l <= heap_size && arr[l] > arr[i]) {
        largest = l;
    }
    else {
        largest = i;
    }

    oph++;
    if (r <= heap_size && arr[r] > arr[largest]) {
        largest = r;
    }

    if (largest != i) {
        oph += 3;
        swap(arr[i], arr[largest]);
        maxHeapify(arr, largest);
    }
}

void buildMaxHeapBU(int arr[], int n) {
    heap_size = n - 1;
    for (int i = n / 2 - 1; i >= 0; i--) {
        maxHeapify(arr, i);
    }
}

void heapSort(int arr[], int n) {
    buildMaxHeapBU(arr, n);
    for (int i = n - 1; i > 0; i--) {
        oph += 3;
        swap(arr[0], arr[i]);
        heap_size--;
        maxHeapify(arr, 0);
    }
}

//Quicksort algorithm
int partition(int arr[], int p, int r) {
    int x = arr[r];
    opq++;
    int i = p - 1;
    for (int j = p; j < r; j++) {
        opq++;
        if (arr[j] <= x) {
            i++;
            opq += 3;
            swap(arr[i], arr[j]);
        }
    }
    opq += 3;
    swap(arr[i + 1], arr[r]);
    return i + 1;
}

void quickSort(int arr[], int p, int r) {
    int q;

    if (p < r) {
        q = partition(arr, p, r);
        quickSort(arr, p, q - 1);
        quickSort(arr, q + 1, r);
    }
}

//Randomized partitioning and quicksort
int randomizedPartition(int arr[], int p, int r) {
    int i = (rand() % (r - p + 1)) + p;
    swap(arr[r], arr[i]);
    return partition(arr, p, r);
}

void randomizedQuickSort(int arr[], int p, int r) {
    int q;

    if (p < r) {
        q = randomizedPartition(arr, p, r);
        randomizedQuickSort(arr, p, q - 1);
        randomizedQuickSort(arr, q + 1, r);
    }
}

int randomizedSelect(int arr[], int p, int r, int i) {
    if (p == r)
        return arr[p];
    int q, k;
    q = randomizedPartition(arr, p, r);
    k = q - p + 1;
    if (i == k)
        return arr[q];
    else if (i < k)
        return randomizedSelect(arr, p, q - 1, i);
    else return randomizedSelect(arr, q + 1, r, i - k);
}

//Quicksort algorithm
int partitionMiddle(int arr[], int p, int r) {
    int x = (r + p) / 2;
    swap(arr[x], arr[r]);
    x = arr[r];
    opq += 4;
    int i = p - 1;
    for (int j = p; j < r; j++) {
        opq++;
        if (arr[j] <= x) {
            i++;
            opq += 3;
            swap(arr[i], arr[j]);
        }
    }
    opq += 3;
    swap(arr[i + 1], arr[r]);
    return i + 1;
}

void quickSortMiddle(int arr[], int p, int r) {
    int q;

    if (p < r) {
        q = partitionMiddle(arr, p, r);
        quickSortMiddle(arr, p, q - 1);
        quickSortMiddle(arr, q + 1, r);
    }
}

//Test case
void testRun() {

    int orig[16] = { 23, 2, 11, 5856, 2, 2, 121, 10, 30, 40, 50, 1, 1, 421, 33 };
    int n = 15;		//length of array, set it correspondingly
    int tst[16];

    //uncomment next line if wanted to generate random array
    FillRandomArray(orig, n, 1, 1000);

    //prints initial array
    printf("Initial array:\n");
    for (int i = 0; i < n; i++) {
        printf("%d ", orig[i]);
    }
    printf("\nIs it sorted? %s", (IsSorted(tst, n) ? "yes" : "no"));  //these lines verify after each case if the array is sorted

                                                                      //sorts in heapsort then prints array
    copy(orig, tst, 15);
    heapSort(tst, n);
    printf("\n\nHeap sorted array:\n");
    for (int i = 0; i < n; i++) {
        printf("%d ", tst[i]);
    }
    printf("\nIs it sorted? %s\n", (IsSorted(tst, n) ? "yes" : "no"));

    //sorts in quicksort then prints array
    copy(orig, tst, 15);
    quickSort(tst, 0, n - 1);
    printf("\nQuick sorted array:\n");
    for (int i = 0; i < n; i++) {
        printf("%d ", tst[i]);
    }
    printf("\nIs it sorted? %s\n", (IsSorted(tst, n) ? "yes" : "no"));

    //sorts in quicksort then prints array
    copy(orig, tst, 15);
    quickSortMiddle(tst, 0, n - 1);
    printf("\nQuick sorted array (pivot in the middle):\n");
    for (int i = 0; i < n; i++) {
        printf("%d ", tst[i]);
    }
    printf("\nIs it sorted? %s\n", (IsSorted(tst, n) ? "yes" : "no"));

    //sorts in randomized quicksort then prints array
    copy(orig, tst, 15);
    randomizedQuickSort(tst, 0, n - 1);
    printf("\nRandomized Quick sorted array:\n");
    for (int i = 0; i < n; i++) {
        printf("%d ", tst[i]);
    }
    printf("\nIs it sorted? %s\n", (IsSorted(tst, n) ? "yes" : "no"));

    //Randomized select
    copy(orig, tst, 15);
    printf("\nInitial array to select from:\n");
    for (int i = 0; i < n; i++) {
        printf("%d ", tst[i]);
    }
    int rd = randomizedSelect(tst, 0, n - 1, 7);
    printf("\nRandomized Select (7th smallest element): %d\n", rd);
}

int main() {
    int a[MAX_SIZE];
    int b[MAX_SIZE];
    int n;
    int j;

    testRun();

    for (n = 100; n<MAX_SIZE; n += 100) {

        oph = 0;
        opq = 0;

        for (j = 0; j < 5; j++) {
            FillRandomArray(a, n, 1, 100000, false, 0);
            copy(a, b, n);
            //printf("Heapsort %d/%d\n", n, j + 1);
            heapSort(b, n);
            copy(a, b, n);
            //printf("Quicksort %d/%d\n", n, j + 1);
            quickSort(b, 0, n - 1);
        }

        profiler.countOperation("Heapsort average", n, oph / 5);
        profiler.countOperation("Quicksort average", n, opq / 5);

        //Best case
        opq = 0;     
        FillRandomArray(a, n, 1, 100000, true, 1);
        copy(a, b, n);
        quickSortMiddle(b, 0, n - 1);
        profiler.countOperation("Quicksort best", n, opq);

        //Worst case
        opq = 0;
        FillRandomArray(a, n, 1, 100000, true, 1);
        copy(a, b, n);
        quickSort(b, 0, n - 1);
        profiler.countOperation("Quicksort worst", n, opq);
    }

    profiler.createGroup("Total operations", "Heapsort average", "Quicksort average");
    //profiler.createGroup("Worst case", "BottomUp worst", "TopDown worst");

    profiler.showReport();
    //_getch();
    return 0;
}