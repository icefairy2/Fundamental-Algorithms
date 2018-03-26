#include <stdio.h>
#include <stdlib.h>
#include<conio.h>
#include <time.h>

#define N 1007
#define m 3000

/* The overall results are very good, average search takes between 2 and 4 steps, and even at fill factor 
 * 0.99, the value tends to 4 (when the element can actually be found in the table).
 * By steps we mean the number of cells accessed during the search.
 * When the table is almost full (99%) the maximum steps to find an existing element is almost three
 * times larger, but still a decent number.
 * When searching for elements that are not in the table, the number of steps grows significantly,
 * and when the table is 99% full, the maximum of steps to find an element eventually reaches the table size.
 * In conclusion, the use of the hash table is very practical, achieving extreme results only when it 
 * is almost completely full, but even until 95% the searching runs decently fast.
 */

int hash(int key) {
    return key % N;
}

int hashQ(int key, int i) {
    return (hash(key) + i + i*i) % N;
}

int hashInsert(int T[], int key) {
    int j, i = 0;
    do {
        j = hashQ(key, i);
        if (T[j] == 0) {
            T[j] = key;
            return j;
        }
        else
            i++;
    } while (i != N);
    return -1;
}

/* Returns the number of tries to search for an element
 * Returns negative if tries result in element not found
 */
int hashSearch(int T[], int key) {
    int j, i = 0;
    do {
        j = hashQ(key, i);
        if (T[j] == key) {
            return i + 1;     
        }
        i++;
    } while ((T[j] != 0) && (i != N));
    return -i;
}

/* Shows that the hashInsert and the hashSearch functions are correct
 * To call this function, change the value of defined N ( in the header)
 * to a small value, smaller than the length of the array T called on
 */
void testCorrect(int T[]) {
    int i;
    for (int i = 0; i < N; i++)
        T[i] = 0;
    hashInsert(T, 4);
    hashInsert(T, 30);
    hashInsert(T, 13);
    hashInsert(T, 56);
    hashInsert(T, 21);
    hashInsert(T, 41);
    hashInsert(T, 61);
    hashInsert(T, 500);
    printf("Table after insertions:\n");
    for (i = 0; i < N; i++)
        printf("%d. %d, hash function value: %d\n", i, T[i], hash(T[i]));
    int el = 500;
    int hS = hashSearch(T, el);
    printf("Search element: %d\n%s %d\n", el, (hS > 0) ? "Found in" : "Not found", hS);
    el = 56;
    hS = hashSearch(T, el);
    printf("Search element: %d\n%s %d\n", el, (hS > 0) ? "Found in" : "Not found", hS);
    el = 41;
    hS = hashSearch(T, el);
    printf("Search element: %d\n%s %d\n", el, (hS > 0) ? "Found in" : "Not found", hS);
    el = 777;
    hS = hashSearch(T, el);
    printf("Search element: %d\n%s %d\n", el, (hS > 0) ? "Found in" : "Not found", hS);
    el = 81;
    hS = hashSearch(T, el);
    printf("Search element: %d\n%s %d\n", el, (hS > 0) ? "Found in" : "Not found", hS);
}

void testHash(int T[], float fillFactor) {
    int n = N * fillFactor;
    int i, elem, k = 0;
    int searchArr[m/2 + 1];

    for (int i = 0; i < N; i++)
        T[i] = 0;

    //generate elements between 1 and 10,000 to be inserted
    for (i = 0; i < n; i++) {
        elem = 1 + rand() % 10000;
        hashInsert(T, elem);

        //we save some for the search test
        if (i % 5 == 0 && k < m/2) {
            searchArr[k] = elem;
            k++;
        }
    }

    float avgFound = 0, avgNotFound = 0;
    int maxNotFound = 0, maxFound = 0;
    int curr;

    //searching for elements that are surely in the table
    for (i = 0; i < k; i++) {
        curr = hashSearch(T, searchArr[i]);
        avgFound += curr;

        if (curr > maxFound)
            maxFound = curr;
    }
    avgFound /= k;

    //search for elements that are not in the table
    //for this, generate random elements between 10,001 and 20,000
    for (i = 0; i < k; i++) {
        elem = 10001 + rand() % 10000;
        curr = hashSearch(T, elem);
        avgNotFound += curr;

        if (curr < maxNotFound)
            maxNotFound = curr;
    }
    avgNotFound /= k;
    avgNotFound = (-1) * avgNotFound;
    maxNotFound = (-1) * maxNotFound;

    printf("Fill: %1.2f AvgFound: %10.3f MaxFound: %d AvgNotFound: %10.3f MaxNotFound: %d\n",
        fillFactor, avgFound, maxFound, avgNotFound, maxNotFound);
}

int main() {
    int T[N];

    //to call the testCorrect function, change the value of N in the header to be <50
    /*int testArr[50];
    testCorrect(testArr);*/

    testHash(T, 0.8);
    testHash(T, 0.85);
    testHash(T, 0.9);
    testHash(T, 0.95);
    testHash(T, 0.99);

    return 0;
}