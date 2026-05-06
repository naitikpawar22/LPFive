#include <iostream>
#include <vector>
#include <omp.h>
#include <cstdlib>
#include <ctime>

using namespace std;

// ======================= UTILITY =======================
void printArray(vector<int>& arr) {
    for (int x : arr) cout << x << " ";
    cout << endl;
}

// ======================= BUBBLE SORT =======================

// Sequential Bubble Sort
void bubbleSortSeq(vector<int>& arr) {
    int n = arr.size();
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                swap(arr[j], arr[j + 1]);
            }
        }
    }
}

// Parallel Bubble Sort (Odd-Even Sort)
void bubbleSortParallel(vector<int>& arr) {
    int n = arr.size();

    for (int i = 0; i < n; i++) {

        // Even phase
        #pragma omp parallel for
        for (int j = 0; j < n - 1; j += 2) {
            if (arr[j] > arr[j + 1]) {
                swap(arr[j], arr[j + 1]);
            }
        }

        // Odd phase
        #pragma omp parallel for
        for (int j = 1; j < n - 1; j += 2) {
            if (arr[j] > arr[j + 1]) {
                swap(arr[j], arr[j + 1]);
            }
        }
    }
}

// ======================= MERGE SORT =======================

void merge(vector<int>& arr, int left, int mid, int right) {
    vector<int> temp(right - left + 1);

    int i = left, j = mid + 1, k = 0;

    while (i <= mid && j <= right) {
        if (arr[i] <= arr[j]) temp[k++] = arr[i++];
        else temp[k++] = arr[j++];
    }

    while (i <= mid) temp[k++] = arr[i++];
    while (j <= right) temp[k++] = arr[j++];

    for (int i = left, k = 0; i <= right; i++, k++) {
        arr[i] = temp[k];
    }
}

// Sequential Merge Sort
void mergeSortSeq(vector<int>& arr, int left, int right) {
    if (left < right) {
        int mid = (left + right) / 2;

        mergeSortSeq(arr, left, mid);
        mergeSortSeq(arr, mid + 1, right);

        merge(arr, left, mid, right);
    }
}

// Parallel Merge Sort using OpenMP Tasks
void mergeSortParallel(vector<int>& arr, int left, int right) {
    if (left < right) {
        int mid = (left + right) / 2;

        #pragma omp task shared(arr)
        mergeSortParallel(arr, left, mid);

        #pragma omp task shared(arr)
        mergeSortParallel(arr, mid + 1, right);

        #pragma omp taskwait
        merge(arr, left, mid, right);
    }
}

// ======================= MAIN =======================
int main() {
    int n = 10000;  // size of array
    vector<int> arr(n);

    srand(time(0));

    // Generate random data
    for (int i = 0; i < n; i++) {
        arr[i] = rand() % 10000;
    }

    vector<int> arr1 = arr;
    vector<int> arr2 = arr;
    vector<int> arr3 = arr;
    vector<int> arr4 = arr;

    double start, end;

    cout << "Array size: " << n << endl;

    // ================= BUBBLE SORT =================
    start = omp_get_wtime();
    bubbleSortSeq(arr1);
    end = omp_get_wtime();
    cout << "Sequential Bubble Sort Time: " << (end - start) << " seconds\n";

    start = omp_get_wtime();
    bubbleSortParallel(arr2);
    end = omp_get_wtime();
    cout << "Parallel Bubble Sort Time: " << (end - start) << " seconds\n";

    // ================= MERGE SORT =================
    start = omp_get_wtime();
    mergeSortSeq(arr3, 0, n - 1);
    end = omp_get_wtime();
    cout << "Sequential Merge Sort Time: " << (end - start) << " seconds\n";

    start = omp_get_wtime();

    #pragma omp parallel
    {
        #pragma omp single
        mergeSortParallel(arr4, 0, n - 1);
    }

    end = omp_get_wtime();
    cout << "Parallel Merge Sort Time: " << (end - start) << " seconds\n";

    return 0;
}