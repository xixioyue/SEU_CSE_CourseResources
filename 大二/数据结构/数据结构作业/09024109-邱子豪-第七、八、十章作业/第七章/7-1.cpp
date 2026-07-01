#include <algorithm>
#include <iostream>
#include <stdexcept>
#include <vector>

void insertionSort(std::vector<int>& arr) {
    for (size_t i = 1; i < arr.size(); ++i) {
        int key = arr[i];
        size_t j = i;
        while (j > 0 && arr[j - 1] > key) {
            arr[j] = arr[j - 1];
            --j;
        }
        arr[j] = key;
    }
}

void quickSortRange(std::vector<int>& arr, int left, int right) {
    if (left >= right) return;
    int i = left;
    int j = right;
    int pivot = arr[left + (right - left) / 2];
    while (i <= j) {
        while (arr[i] < pivot) ++i;
        while (arr[j] > pivot) --j;
        if (i <= j) {
            std::swap(arr[i], arr[j]);
            ++i;
            --j;
        }
    }
    if (left < j) quickSortRange(arr, left, j);
    if (i < right) quickSortRange(arr, i, right);
}

void quickSort(std::vector<int>& arr) {
    if (!arr.empty()) {
        quickSortRange(arr, 0, static_cast<int>(arr.size()) - 1);
    }
}

void merge(std::vector<int>& arr, int left, int mid, int right, std::vector<int>& buf) {
    int i = left;
    int j = mid + 1;
    int k = left;
    while (i <= mid && j <= right) {
        if (arr[i] <= arr[j]) {
            buf[k++] = arr[i++];
        } else {
            buf[k++] = arr[j++];
        }
    }
    while (i <= mid) buf[k++] = arr[i++];
    while (j <= right) buf[k++] = arr[j++];
    for (int idx = left; idx <= right; ++idx) {
        arr[idx] = buf[idx];
    }
}

void mergeSortRange(std::vector<int>& arr, int left, int right, std::vector<int>& buf) {
    if (left >= right) return;
    int mid = left + (right - left) / 2;
    mergeSortRange(arr, left, mid, buf);
    mergeSortRange(arr, mid + 1, right, buf);
    merge(arr, left, mid, right, buf);
}

void mergeSort(std::vector<int>& arr) {
    if (arr.empty()) return;
    std::vector<int> buffer(arr.size());
    mergeSortRange(arr, 0, static_cast<int>(arr.size()) - 1, buffer);
}

void heapify(std::vector<int>& arr, int n, int i) {
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;
    if (left < n && arr[left] > arr[largest]) largest = left;
    if (right < n && arr[right] > arr[largest]) largest = right;
    if (largest != i) {
        std::swap(arr[i], arr[largest]);
        heapify(arr, n, largest);
    }
}

void heapSort(std::vector<int>& arr) {
    int n = static_cast<int>(arr.size());
    for (int i = n / 2 - 1; i >= 0; --i) {
        heapify(arr, n, i);
    }
    for (int i = n - 1; i > 0; --i) {
        std::swap(arr[0], arr[i]);
        heapify(arr, i, 0);
    }
}

void radixSort(std::vector<int>& arr, int r) {
    if (r < 2) throw std::invalid_argument("Radix r must be >= 2");
    if (arr.empty()) return;
    for (int v : arr) {
        if (v < 0) throw std::invalid_argument("Radix sort assumes non-negative integers");
    }
    int maxVal = *std::max_element(arr.begin(), arr.end());
    std::vector<int> output(arr.size());
    for (int exp = 1; maxVal / exp > 0; exp *= r) {
        std::vector<int> count(r, 0);
        for (int v : arr) {
            int digit = (v / exp) % r;
            ++count[digit];
        }
        for (int i = 1; i < r; ++i) {
            count[i] += count[i - 1];
        }
        for (int i = static_cast<int>(arr.size()) - 1; i >= 0; --i) {
            int digit = (arr[i] / exp) % r;
            output[--count[digit]] = arr[i];
        }
        arr = output;
    }
}

void printArray(const std::vector<int>& arr) {
    for (size_t i = 0; i < arr.size(); ++i) {
        if (i) std::cout << " ";
        std::cout << arr[i];
    }
    std::cout << "\n";
}

int main() {
    std::vector<int> data{42, 17, 8, 99, 1, 23, 17, 50, 65, 4};

    std::vector<int> insertionData = data;
    insertionSort(insertionData);
    std::cout << "Insertion Sort: ";
    printArray(insertionData);

    std::vector<int> quickData = data;
    quickSort(quickData);
    std::cout << "Quick Sort: ";
    printArray(quickData);

    std::vector<int> mergeData = data;
    mergeSort(mergeData);
    std::cout << "Merge Sort: ";

    printArray(mergeData);
    std::vector<int> heapData = data;
    heapSort(heapData);
    std::cout << "Heap Sort: ";
    printArray(heapData);

    std::vector<int> radixData = data;
    radixSort(radixData, 10);
    std::cout << "Radix-10 Sort: ";
    printArray(radixData);

    return 0;
}
