#include <iostream>
#include <vector>
#include <random>
#include <fstream>


void merge(std::vector<int> &v, int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    std::vector<int> L(n1), R(n2);

    for (int i = 0; i < n1; i++)
        L[i] = v[left + i];
    for (int j = 0; j < n2; j++)
        R[j] = v[mid + 1 + j];

    int i = 0, j = 0;
    int k = left;

    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            v[k] = L[i];
            ++i;
        }
        else {
            v[k] = R[j];
            ++j;
        }
        ++k;
    }
    while (i < n1) {
        v[k] = L[i];
        ++i;
        ++k;
    }
    while (j < n2) {
        v[k] = R[j];
        ++j;
        ++k;
    }
}
void mergeSort(std::vector<int> &v, int left, int right)
{
    if (left >= right)
        return;

    int mid = left + (right - left) / 2;
    mergeSort(v, left, mid);
    mergeSort(v, mid + 1, right);
    merge(v, left, mid, right);
}

void insertionSort(std::vector<int> &v) {
    for (int i = 1; i < (int)v.size(); ++i) {
        int key = v[i];
        int j = i - 1;
        
        while (j >= 0 && v[j] > key) {
            v[j + 1] = v[j];
            j = j - 1;
        }
        v[j + 1] = key;
    }
}


class ArrayGenerator {
public:

    ArrayGenerator() : rng(std::random_device{}()) {}

    std::vector<int> generateRandomArray(int size) {
        std::vector<int> array(size);
        std::uniform_int_distribution<int> dist(0, 6000);

        for (int& elem : array) {
            elem = dist(rng);
        }
        return array;
    }

    std::vector<int> generateReverseSortedArray(int size) {
        std::vector<int> array = generateRandomArray(size);
        std::sort(array.begin(), array.end(), std::greater<int>());
        return array;
    }


    std::vector<int> generateAlmostSortedArray(int size) {
        std::vector<int> array(size);
        for (int i = 0; i < size; ++i) {
            array[i] = i;
        }

        std::shuffle(array.begin(), array.end(), rng);
        for (int i = 0; i < 10; ++i) {
            int index1 = rng() % size;
            int index2 = rng() % size;
            std::swap(array[index1], array[index2]);
        }

        return array;
    }

private:
    std::mt19937 rng;
};

class SortTester {
public:

    void insertionSort(std::vector<int> &v, int left, int right) {
        for (int i = left + 1; i <= right; ++i) {
            int key = v[i];
            int j = i - 1;
            while (j >= left && v[j] > key) {
                v[j + 1] = v[j];
                j--;
            }
            v[j + 1] = key;
        }
    }

    void mergeInsertionSort(std::vector<int> &v, int left, int right, int threshold) {
        if (right - left + 1 <= threshold) {
            insertionSort(v, left, right);
            return;
        }
        if (left < right) {
            int mid = left + (right - left) / 2;
            mergeInsertionSort(v, left, mid, threshold);
            mergeInsertionSort(v, mid + 1, right, threshold);
            merge(v, left, mid, right);
        }
    }


    void merge(std::vector<int> &v, int left, int mid, int right) {
        int n1 = mid - left + 1;
        int n2 = right - mid;

        std::vector<int> L(n1), R(n2);
        for (int i = 0; i < n1; i++) L[i] = v[left + i];
        for (int j = 0; j < n2; j++) R[j] = v[mid + 1 + j];

        int i = 0, j = 0, k = left;
        while (i < n1 && j < n2) {
            if (L[i] <= R[j]) v[k++] = L[i++];
            else v[k++] = R[j++];
        }
        while (i < n1) v[k++] = L[i++];
        while (j < n2) v[k++] = R[j++];
    }


    void testSorting(int threshold, std::ofstream &outFile) {
        ArrayGenerator generator;
        for (int size = 500; size <= 10000; size += 100) {

            auto randomArray = generator.generateRandomArray(size);
            auto reverseSortedArray = generator.generateReverseSortedArray(size);
            auto almostSortedArray = generator.generateAlmostSortedArray(size);

           
            auto start = std::chrono::high_resolution_clock::now();
            mergeInsertionSort(randomArray, 0, size - 1, threshold);
            auto elapsed = std::chrono::high_resolution_clock::now() - start;
            long long randomTime = std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();


            start = std::chrono::high_resolution_clock::now();
            mergeInsertionSort(reverseSortedArray, 0, size - 1, threshold);
            elapsed = std::chrono::high_resolution_clock::now() - start;
            long long reverseTime = std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();

       
            start = std::chrono::high_resolution_clock::now();
            mergeInsertionSort(almostSortedArray, 0, size - 1, threshold);
            elapsed = std::chrono::high_resolution_clock::now() - start;
            long long almostSortedTime = std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();

            outFile << size << "," << threshold << "," << randomTime << "," << reverseTime << "," << almostSortedTime << "\n";
        }
    }
};

int main() {
    std::ofstream outFile("/Users/mac/Desktop/alogoses/merge_insertion_sort.csv");
    outFile << "Size,Threshold,RandomArray,ReverseSortedArray,AlmostSortedArray\n";

    SortTester tester;
    for (int threshold : {40, 50, 60, 70, 80}) {
        tester.testSorting(threshold, outFile);
    }

    outFile.close();
    return 0;
}
