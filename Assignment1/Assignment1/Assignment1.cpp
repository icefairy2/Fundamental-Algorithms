#include <stdio.h>
#include <conio.h>
#include "Profiler.h"

#define MAX_SIZE 10000

/**
 * In the BEST CASE, when the array is already sorted:
 *   1. Assignments: the numbers for BUBBLE and SELECTION sort stay constant 0; they both rely on swaps
 *					 which take place only when a comparison between two elements returns true (it est they are not in order).
 *					 The number for INSERTION sort however is 2*(size-1) because the 'key' is assigned an element and then
 *					 later on the element is reassigned the value of the key; this number grows linearly.
 *	2. Comparisons: the numbers for BUBBLE sort are (size-1) and for INSERTION sort (size-2) (->linearity) which is why they 
 *					show up so close to each other on the chart, and they are also illustrated on a separate one aswell, because
 *					the number of comparisons for SELECTION sort is significantly larger: (size*(size-1)/2) that is, the sum 
 *					of natural numbers from 1 to size-1; every element gets compared to	all the elements above it.
 *	3. Operations in general: on the chart with the sum of operations (assignments + comparisons) in best case, the 
 *					SELECTION sort operations show up way above the the others, that is why the other two are also displayed 
 *					on a separate chart, where we can observe again the linear growth of the numbers.
 *
 * In the WORST CASE, when the array is inversely sorted:
 * (there may be other bad cases ex. at selection sort but we take the inversely sorted array as being the worst)
 *   1. Assignments: as one should expect, the number of assignments is the worst for the BUBBLE sort, because to reverse the
 *                   array, all the elements are swapped one-by-one until they get to their place on (almost) the other end of the array.
 *					 The number for SELECTION sort however is fairly small, lying way below the other two, and also displayed 
 *					 on a separate chart where as we can observe it is shown that it grows linearly.
 *	2. Comparisons: the number of comparisons in the worst case is almost equal for all three algorithms, the numbers for 
 *					BUBBLE and SELECTION sort are actually equal, being (size*(size-1)/2), and only the number for INSERTION
 *					sort is a bit bigger. They overlap each other on the chart, that is why they cannot be distinguished at first sight.
 *	3. Operations in general: on the chart with the sum of operations (assignments + comparisons) in worst case, all three
 *					can be beautifully distinguished, the lowest numbers are at SELECTION sort, then INSERTION sort, and BUBBLE sort
 *					performs the highest number of operations on an array of fixed size.
 *
 * In the AVERAGE CASE, when the array is randomized, the numbers displayed are the 5*(the number of operations for the array 
 * of current size) from running the algorithms on 5 different random arrays.
 * To get the actual average value, all the numbers should be divided by 5:
 *   1. Assignments: the numbers are for example B:36669 I:13213 S:1425 on an array of size 100 which would mean the average values
 *					 are B:7333 I:2642 S:285 so numbers mostly rely between best and worst cases.
 *					 Number is highest for BUBBLE sort, smallest for SELECTION sort, the latter being also displayed separately
 *					 so it can be clearly observed.
 *	2. Comparisons: the numbers are for example B:24232 I:13055 S:24750 on an array of size 100 which would mean the average values
 *					are B:4846 I:2611 S:4950 so numbers mostly rely between best and worst cases.
 *					Number stays constant for SELECTION sort, because it only depends on the length of the array.
 *					Comparisons for BUBBLE sort also tend to the constant number at selection sort (size*(size-1)/2)
 *					but it is not exactly the same because in this implementation of bubble sort, if at one step more elements
 *					get to their place, these are not compared again. On the chart they show up very close to each other.
 *	3. Operations in general: the numbers are for example B:60532 I:26110 S:26190 on an array of size 100 which would mean the average values
 *					 are B:12106 I:5222 S:5238 so numbers mostly rely between best and worst cases.
 *					 The numbers for INSERTION and SELECTION sort show up very close to each other that is why they cannot be 
 *					 clearly distinguished on the chart.
 */

Profiler profiler("Sorting");


//Bubble sort algorithm
void bubbleSort(int arr[], int n, char caseComp[], char caseAssign[]) {
    int a=0, c=0;
    int tmp, newn, tempn = n, i;
    while (tempn != 0) {
        newn = 0;				
        for (i = 1; i<tempn; i++) {
            c++;
            if (arr[i - 1] > arr[i]) {
                tmp = arr[i - 1];
                arr[i - 1] = arr[i];
                arr[i] = tmp;
                a += 3;
                newn = i;
            }
        }
        tempn = newn;     //if at one step more elements got to their place, we skip comparing them again
    }
    profiler.countOperation(caseAssign, n, a);
    profiler.countOperation(caseComp, n, c);
}

//Insertion sort algorithm
void insertionSort(int arr[], int n, char caseComp[], char caseAssign[]) {
    int a = 0, c = 0;
    int key = 0, i=0, j;
    for (j = 1; j < n; j++) {
        a++;
        key = arr[j];
        i = j - 1;
        while (arr[i] > key && i >= 0) {
            a++;
            c++;
            arr[i + 1] = arr[i];
            i--;
        }
        if (i != 0)
            c++;
        a++;
        arr[i + 1] = key;
    }
    profiler.countOperation(caseAssign, n, a);
    profiler.countOperation(caseComp, n, c);
}

//Selection sort algorithm
void selectionSort(int arr[], int n, char caseComp[], char caseAssign[]) {
    int a = 0, c = 0;
    int tmp, iMin, i, j;
    for (j = 0; j < n - 1; j++) {
        iMin = j;
        for (i = j + 1; i < n; i++) {
            c++;
            if (arr[i] < arr[iMin]) {
                iMin = i;
            }
        }
        if (iMin != j) {
            tmp = arr[j];
            arr[j] = arr[iMin];
            arr[iMin] = tmp;
            a += 3;
        }
    }
    profiler.countOperation(caseAssign, n, a);
    profiler.countOperation(caseComp, n, c);
}

/**
 * Test case for the three algorithms
 * takes hardcoded array (or generates a random one if requested)
 * sorts it with bubble, insertion and selection sort
 * at each case also verifies and displays if the array is sorted
 */
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
    bubbleSort(tst, n, "Test comp bubble", "Test assign bubble");
    printf("\n\nBubble sorted array:\n");
    for (int i = 0; i < n; i++) {
        printf("%d ", tst[i]);
    }
    printf("\nIs it sorted? %s", (IsSorted(tst, n) ? "yes" : "no"));

    //sorts in insertionsort then prints array
    insertionSort(tst, n, "Test comp insert", "Test assign insert");
    printf("\n\nInsertion sorted array:\n");
    for (int i = 0; i < n; i++) {
        printf("%d ", tst[i]);
    }
    printf("\nIs it sorted? %s", (IsSorted(tst, n) ? "yes" : "no"));

    //sorts in selectionsort then prints array
    selectionSort(tst, n, "Test comp select", "Test assign select");
    printf("\n\nSelection sorted array:\n");
    for (int i = 0; i < n; i++) {
        printf("%d ", tst[i]);
    }
    printf("\nIs it sorted? %s", (IsSorted(tst, n) ? "yes" : "no"));
}

/**
 * Runs all three algorithms in:
 *	1. best cases - array already sorted
 *	2. worst cases - array reverse sorted
 *	3. 5 times average cases - array randomized, 
 *	   operation counters retain the sum of the 5 case operation numbers
 * Uncommenting lines containing Timer references will measure also 
 * the time taken by the algorithms for every n(length of array).
 */
void sortRun(int arr[]) {

    //Best case scenario
    FillRandomArray(arr, MAX_SIZE, 0, MAX_SIZE, false, 1);

    for (int n = 100; n <= MAX_SIZE; n += 100) {

        printf("bubble sorting best n=%d \n", n);
        //profiler.startTimer("Bubble sort best", n);
        bubbleSort(arr, n, "Bubble comparisons best", "Bubble assignments best");
        //profiler.stopTimer("Bubble sort best", n);

        printf("insert sorting best n=%d \n", n);
        //profiler.startTimer("Insertion sort best", n);
        insertionSort(arr, n, "Insert comparisons best", "Insert assignments best");
        //profiler.stopTimer("Insertion sort best", n);

        printf("select sorting best n=%d \n", n);
        //profiler.startTimer("Selection sort best", n);
        selectionSort(arr, n, "Select comparisons best", "Select assignments best");
        //profiler.stopTimer("Selection sort best", n);
    }
    
    //Worst case scenario
    FillRandomArray(arr, MAX_SIZE, 0, MAX_SIZE, false, 2);
    
    /*At every n and sorting method we'll copy into temp the part 
      of the original array that interests us and sort temp*/
    int temp[MAX_SIZE];	

    for (int n = 100; n <= MAX_SIZE; n += 100) {

        for (int i = 0; i < n; i++)
            temp[i] = arr[i];

        printf("bubble sorting worst n=%d \n", n);
        //profiler.startTimer("Bubble sort worst", n);
        bubbleSort(temp, n, "Bubble comparisons worst", "Bubble assignments worst");
        //profiler.stopTimer("Bubble sort worst", n);

        for (int i = 0; i < n; i++)
            temp[i] = arr[i];

        printf("insert sorting worst n=%d \n", n);
        //profiler.startTimer("Insertion sort worst", n);
        insertionSort(temp, n, "Insert comparisons worst", "Insert assignments worst");
        //profiler.stopTimer("Insertion sort worst", n);

        for (int i = 0; i < n; i++)
            temp[i] = arr[i];
        //selection sort algorithm is unstable when it comes to equal elements so this may not be the worst case
        //another selection sort worst case: array mostly sorted but minimum is on the last position

        printf("select sorting worst n=%d \n", n);
        //profiler.startTimer("Selection sort worst", n);
        selectionSort(temp, n, "Select comparisons worst", "Select assignments worst");
        //profiler.stopTimer("Selection sort worst", n);
    }
    
    //Average cases, ran 'cases' times (5 for instance)
    for (int cases = 1; cases <= 5; cases++) {
        
        FillRandomArray(arr, MAX_SIZE, 0, MAX_SIZE, false, 0);
        
        for (int n = 100; n <= MAX_SIZE; n += 100) {
    
            for (int i = 0; i < n; i++)
                temp[i] = arr[i];

            printf("bubble sorting average n=%d \n", n);
            //profiler.startTimer("Bubble sort average", n);
            bubbleSort(temp, n, "Bubble comparisons average", "Bubble assignments average");
            //profiler.stopTimer("Bubble sort average", n);

            for (int i = 0; i < n; i++)
                temp[i] = arr[i];

            printf("insert sorting average n=%d \n", n);
            //profiler.startTimer("Insertion sort average", n);
            insertionSort(temp, n, "Insert comparisons average", "Insert assignments average");
            //profiler.stopTimer("Insertion sort average", n);

            for (int i = 0; i < n; i++)
                temp[i] = arr[i];

            printf("select sorting average n=%d \n", n);
            //profiler.startTimer("Selection sort average", n);
            selectionSort(temp, n, "Select comparisons average", "Select assignments average");
            //profiler.stopTimer("Selection sort average", n);
        }
    }
}

//This procedure creates the analysis charts
void createCharts() {
    profiler.addSeries("Insert operations best", "Insert assignments best", "Insert comparisons best");
    profiler.addSeries("Select operations best", "Select assignments best", "Select comparisons best");
    profiler.addSeries("Bubble operations best", "Bubble assignments best", "Bubble comparisons best");

    profiler.addSeries("Insert operations worst", "Insert assignments worst", "Insert comparisons worst");
    profiler.addSeries("Select operations worst", "Select assignments worst", "Select comparisons worst");
    profiler.addSeries("Bubble operations worst", "Bubble assignments worst", "Bubble comparisons worst");

    profiler.addSeries("Insert operations average", "Insert assignments average", "Insert comparisons average");
    profiler.addSeries("Select operations average", "Select assignments average", "Select comparisons average");
    profiler.addSeries("Bubble operations average", "Bubble assignments average", "Bubble comparisons average");

    profiler.createGroup("Assignments best", "Bubble assignments best", "Insert assignments best",  "Select assignments best");
    profiler.createGroup("Assignments worst", "Bubble assignments worst","Insert assignments worst",  "Select assignments worst");
    profiler.createGroup("Select assignment worst", "Select assignments worst");
    profiler.createGroup("Assignments average", "Bubble assignments average","Insert assignments average",  "Select assignments average");
    profiler.createGroup("Select assignment average", "Select assignments average");
    
    profiler.createGroup("Comparisons best", "Bubble comparisons best", "Insert comparisons best", "Select comparisons best");
    profiler.createGroup("Bubble-Insert comparisons best", "Bubble comparisons best", "Insert comparisons best");
    profiler.createGroup("Comparisons worst", "Bubble comparisons worst", "Insert comparisons worst", "Select comparisons worst");
    profiler.createGroup("Comparisons average", "Bubble comparisons average","Insert comparisons average",  "Select comparisons average");
    
    profiler.createGroup("Operations best", "Bubble operations best", "Insert operations best", "Select operations best");
    profiler.createGroup("Bubble-Insert operations best", "Bubble operations best", "Insert operations best");
    profiler.createGroup("Operations worst", "Bubble operations worst", "Insert operations worst", "Select operations worst");
    profiler.createGroup("Operations average", "Bubble operations average", "Insert operations average", "Select operations average");

    //Uncomment next section if timers were also generated
    /*profiler.createGroup("Best running times", "Insertion sort best", "Bubble sort best", "Select sort best");
    profiler.createGroup("Average running times", "Insertion sort average", "Bubble sort average", "Select sort average");
    profiler.createGroup("Worst running times", "Insertion sort worst", "Bubble sort worst", "Select sort worst");*/
}

int main()
{
    int arr[MAX_SIZE];	

    testRun();

    sortRun(arr);
    createCharts();
    profiler.showReport();

    _getch();
    return 0;
}