#include<iostream>
#include<fstream>
#include<sstream>
#include<string>
#include<time.h>
#include<chrono>
#include<ctime>

using namespace std;
using namespace std::chrono;

// GLOBAL VARIABLES - START //
double partialSelectDupRunningTime;
double partialSelectRunningTime;
double bubbleSelectionRunningTime;
double randSelectRunningTime;
// GLOBAL VARIABLES - END //

// SUPPLEMENTARY FUNCTIONS START //
void printArray(int array[], int arrSize, int newlineSize) {
    for(int i = 0; i < arrSize; i++) {
        cout << array[i] << " ";
        if ((i+1) % newlineSize == 0 && i != 0) {
            cout << "\n";
        }
    }
    cout << "\n";
}

void printSizeOf(int &myInt) {
    cout << "Size (Integer): " << sizeof(myInt) << endl << endl;
}

void printSizeOf(bool &myBool) {
    cout << "Size (Bool): " << sizeof(myBool) << endl << endl;
}
// SUPPLEMENTARY FUNCTIONS END //
// RANDOMIZED SELECT START //
int Partition(int PArray[], int Pstart, int Pend) {
    int x = PArray[Pend];
    int i = Pstart - 1;
    for(int j = Pstart; j < Pend; j++) {
        if (PArray[j] <= x) {
            i = i + 1;
            int temp = PArray[i];
            PArray[i] = PArray[j];
            PArray[j] = temp;
        }
    }
    int temp = PArray[i+1];
    PArray[i+1] = PArray[Pend];
    PArray[Pend] = temp;
    return i+1;
}

int RandomizedPartition(int RPArray[], int RPstart, int RPend) {
    int i = rand()%(RPend-RPstart + 1) + RPstart;
    int temp = RPArray[RPend];
    RPArray[RPend] = RPArray[i];
    RPArray[i] = temp;
    return Partition(RPArray, RPstart, RPend);
}

int RandomizedSelect(int Array[], int start, int end, int target) {
    if (start == end) {
        return Array[start];
    }
    int pivot = RandomizedPartition(Array, start, end);
    int k = pivot - start+1;
    if (target == k) {
        return Array[pivot];
    } else if (target < k) {
        return RandomizedSelect(Array, start, pivot-1, target);
    } else {
        return RandomizedSelect(Array, pivot+1, end, target-k);
    }
}
// RANDOMIZED SELECT END //
// SELECTION BY SORTING - BUBBLESORT START //
// From: https://www.tutorialspoint.com/data_structures_algorithms/bubble_sort_algorithm.htm & https://www.geeksforgeeks.org/bubble-sort/
void BubbleSort(int Array[], int n) {
    for(int i = 0; i < n - 1; i++) {
        for(int j = 0; j < n-i-1; j++) {
            if (Array[j] > Array[j+1]) {
                int temp = Array[j];
                Array[j] = Array[j+1];
                Array[j+1] = temp;
            }
        }
    }
}

void BubbleSortOptimized(int Array[], int n) {
    bool swapped;
    for(int i = 0; i < n - 1; i++) {
        swapped = false;
        for(int j = 0; j < n-i-1; j++) {
            if (Array[j] > Array[j+1]) {
                int temp = Array[j];
                Array[j] = Array[j+1];
                Array[j+1] = temp;
                swapped = true;
            }
        }
        // If no swap occurred in the inner-loop, then break, as the remainder must be in sorted order.
        if(swapped == false) {
            break;
        }
    }
}
// SELECTION BY SORTING - BUBBLESORT END //
// SELECTION BY SORTING - PARTIAL SELECTION SORT START //
// From: https://en.wikipedia.org/wiki/Selection_algorithm
int PartialSelectionSort(int Array[], int n, int target) {
    for(int i = 0; i < target; i++) {
        int minIndex = i;
        int minValue = Array[i];
        for(int j = i+1; j < n; j++) {
            if (Array[j] < minValue) {
                minIndex = j;
                minValue = Array[j];
            }
        }
        int temp = Array[i];
        Array[i] = Array[minIndex];
        Array[minIndex] = temp;
    }
    return Array[target-1];
}

int PartialSelectionSortDupAware(int Array[], int n, int target) {
    int counter = 0;
    int i = 0;
    while(counter != target) {
        if (i == n) {
            cout << "Error: the target order statistic is not available in this array, possibly due to duplicates." << endl;
            return 0;
        }
        int minIndex = i;
        int minValue = Array[i];
        for(int j = i+1; j < n; j++) {
            if (Array[j] < minValue) {
                minIndex = j;
                minValue = Array[j];
            }
        }
        int temp = Array[i];
        Array[i] = Array[minIndex];
        Array[minIndex] = temp;
        if (i == 0 || Array[i-1] < Array[i])
        {
            counter++;
        }
        i++;
    }
    return Array[i-1];
}
// SELECTION BY SORTING - PARTIAL SELECTION SORT END //

int main() {

    steady_clock::time_point setupTimeStart = steady_clock::now();
    srand(time(0));
    const int dataSize = 1000; // Sizes in report are 100, 1.000, and 10.000
    int OriginalArray[dataSize];
    int SortArray[dataSize];
    int PartialSortArray[dataSize];
    int PartialSortDupArray[dataSize];
    fstream dataSet("data.csv");
    if (dataSet.is_open()) {
        for(int i = 0; i < dataSize; i++) {
            string lineElement;
            getline(dataSet, lineElement, ',');
            int tempHolder = stoi(lineElement);
            OriginalArray[i] = tempHolder;
            SortArray[i] = tempHolder;
            PartialSortArray[i] = tempHolder;
            PartialSortDupArray[i] = tempHolder;

        }
    }
    steady_clock::time_point setupTimeStop = steady_clock::now();

    auto setupDuration = setupTimeStop - setupTimeStart;
    cout << "--- Data Size: " << dataSize << " ---" << endl;
    cout << "--- Setup Time: " << duration <double, milli> (setupDuration).count() << " ms " << " ---" << endl << endl;

    cout << "--- Original Array ---" << endl;
    printArray(OriginalArray, dataSize, 10);

     // VARIABLES START//
    int inputStart = 0;
    int inputEnd = dataSize-1;
    int inputTarget = 500;

    bool randSel = true;
    bool bubbleSel = true;
    bool partSortSel = true;
    bool partSortSelDupAware = false;

    bool repeatTimings = true;
    // VARIABLES END //

    cout << "--- Target: " << inputTarget << " ---" << endl << endl;

    if (randSel) {
        cout << "--- Randomized Select ---" << endl;
        steady_clock::time_point randSelectStart = steady_clock::now();
        cout << RandomizedSelect(OriginalArray, inputStart, inputEnd, inputTarget) << endl << endl;
        steady_clock::time_point randSelectStop = steady_clock::now();

        auto randSelectDuration = randSelectStop - randSelectStart;
        randSelectRunningTime = duration <double, milli> (randSelectDuration).count();

        cout << "--- Randomized Select Running Time: " << randSelectRunningTime << " ms " << " ---" << endl << endl;

        cout << "--- Original Array - After Random Select ---" << endl;
        printArray(OriginalArray, dataSize, 10);
    }
    
    if (bubbleSel) {
        cout << "--- Selection by Bubble Sort ---" << endl;
        steady_clock::time_point bubbleStart = steady_clock::now();
        BubbleSort(SortArray, dataSize);
        cout << SortArray[inputTarget-1] << endl << endl;
        steady_clock::time_point bubbleStop = steady_clock::now();

        auto bubbleDuration = bubbleStop - bubbleStart;
        bubbleSelectionRunningTime = duration <double, milli> (bubbleDuration).count();

        cout << "--- Bubble Sort Running Time: " << bubbleSelectionRunningTime << " ms " << " ---" << endl << endl;

        cout << "--- Sorted Array---" << endl;
        printArray(SortArray, dataSize, 10);
    }

    if (partSortSel) {
        cout << "--- Selection by Partial Selection Sort ---" << endl;
        steady_clock::time_point partialSelectStart = steady_clock::now();
        cout << PartialSelectionSort(PartialSortArray, dataSize, inputTarget) << endl << endl;
        steady_clock::time_point partialSelectStop = steady_clock::now();

        auto partialSelectDuration = partialSelectStop - partialSelectStart;
        partialSelectRunningTime = duration <double, milli> (partialSelectDuration).count();

        cout << "--- Partial Selection Sort Running Time: " << partialSelectRunningTime << " ms " << " ---" << endl << endl;

        cout << "--- Partially Sorted Array ---" << endl;
        printArray(PartialSortArray, dataSize, 10);
    }

    if (partSortSelDupAware) {
        cout << "--- Selection by Partial Selection Sort ---" << endl;
        steady_clock::time_point partialSelectDupStart = steady_clock::now();
        cout << PartialSelectionSortDupAware(PartialSortDupArray, dataSize, inputTarget) << endl << endl;
        steady_clock::time_point partialSelectDupStop = steady_clock::now();

        auto partialSelectDupDuration = partialSelectDupStop - partialSelectDupStart;
        partialSelectDupRunningTime = duration <double, milli> (partialSelectDupDuration).count();

        cout << "--- Partial Selection Sort (Duplicate Aware) Running Time: " << partialSelectDupRunningTime << " ms " << " ---" << endl << endl;

        cout << "--- Partially Sorted Array ---" << endl;
        printArray(PartialSortDupArray, dataSize, 10);
    }

    if (repeatTimings) {

        cout << "--- Setup Time: " << duration <double, milli> (setupDuration).count() << " ms " << " ---" << endl << endl;

        if (randSel) {
            cout << "--- Randomized Select Running Time: " << randSelectRunningTime << " ms " << " ---" << endl << endl;
        }
        if (bubbleSel) {
            cout << "--- Bubble Sort Running Time: " << bubbleSelectionRunningTime << " ms " << " ---" << endl << endl;
        }
        if (partSortSel) {
            cout << "--- Partial Selection Sort Running Time: " << partialSelectRunningTime << " ms " << " ---" << endl << endl;
        }
        if (partSortSelDupAware) {
            cout << "--- Partial Selection Sort (Duplicate Aware) Running Time: " << partialSelectDupRunningTime << " ms " << " ---" << endl << endl;
        }
    }

    return 0;
}