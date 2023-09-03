#include <iostream>
#include <vector>
#include <thread>
#include <future>

void merge(int* arr, int l, int m, int r) {
    int nl = m - l + 1;
    int nr = r - m;

    int* left = new int[nl];
    int* right = new int[nr];

    for (int i = 0; i < nl; i++)
        left[i] = arr[l + i];
    for (int j = 0; j < nr; j++)
        right[j] = arr[m + 1 + j];

    int i = 0, j = 0;
    int k = l;

    while (i < nl && j < nr) {
        if (left[i] <= right[j]) {
            arr[k] = left[i];
            i++;
        }
        else {
            arr[k] = right[j];
            j++;
        }
        k++;
    }

    while (i < nl) {
        arr[k] = left[i];
        i++;
        k++;
    }

    while (j < nr) {
        arr[k] = right[j];
        j++;
        k++;
    }

    delete[] left;
    delete[] right;
}

void mergeSort(int* arr, int l, int r, int maxThreads) {
    if (l >= r)
        return;

    int m = (l + r - 1) / 2;
    if (maxThreads > 0 && r - l >= maxThreads) {
        // Если количество элементов в подмассиве больше или равно maxThreads, 
        // разбиваем сортировку на две части и запускаем асинхронно
        std::future<void> leftSort = std::async(std::launch::async, [&]() {
            mergeSort(arr, l, m, maxThreads / 2); // Ограничиваем количество параллельно работающих потоков для сортировки левой части массива.
            });
        mergeSort(arr, m + 1, r, maxThreads / 2); // Ограничиваем количество параллельно работающих потоков для сортировки правой части массива.

        // Дожидаемся завершения сортировки левой части
        leftSort.get();
    }
    else {
        // Если не ограничено количество потоков, запускаем обе части синхронно
        mergeSort(arr, l, m, maxThreads);
        mergeSort(arr, m + 1, r, maxThreads);
    }

    merge(arr, l, m, r);
}

int main() {
    int arr[] = { 2, 1, 10, -5, 40, -20, 0, 4 };
    int n = sizeof(arr) / sizeof(arr[0]);
    const int maxThreads = 2; // Максимальное количество одновременно запущенных потоков

    std::cout << "Original Array:" << std::endl;
    for (int i = 0; i < n; i++) {
        std::cout << arr[i] << " ";
    }
    std::cout << std::endl;

    mergeSort(arr, 0, n - 1, maxThreads);

    std::cout << "Sorted Array:" << std::endl;
    for (int i = 0; i < n; i++) {
        std::cout << arr[i] << " ";
    }
    std::cout << std::endl;

    return 0;
}
