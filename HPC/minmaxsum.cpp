#include <iostream>
#include <vector>
#include <omp.h>
#include <limits.h>

using namespace std;

int main() {
    int n;

    cout << "Enter number of elements: ";
    cin >> n;

    vector<int> arr(n);

    cout << "Enter elements:\n";
    for (int i = 0; i < n; i++) {
        cin >> arr[i];
    }

    int sum = 0;
    int min_val = INT_MAX;
    int max_val = INT_MIN;

    // ================= PARALLEL REDUCTION =================
    #pragma omp parallel for reduction(+:sum) reduction(min:min_val) reduction(max:max_val)
    for (int i = 0; i < n; i++) {
        sum += arr[i];

        if (arr[i] < min_val)
            min_val = arr[i];

        if (arr[i] > max_val)
            max_val = arr[i];
    }

    double avg = (double)sum / n;

    // ================= OUTPUT =================
    cout << "\nResults using Parallel Reduction:\n";
    cout << "Sum = " << sum << endl;
    cout << "Minimum = " << min_val << endl;
    cout << "Maximum = " << max_val << endl;
    cout << "Average = " << avg << endl;

    return 0;
}