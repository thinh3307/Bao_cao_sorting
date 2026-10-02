#include <algorithm>
#include <chrono>
#include <cstdio>
#include <random>
#include <string>
#include <vector>
using namespace std;
using namespace std::chrono;

// ====== SUA 2 DUONG DAN NAY CHO DUNG MAY BAN (dung dau / ) ======
  const string DEFAULT_DATA_DIR = "data";
  const string DEFAULT_CSV      = "results.csv";
// =================================================================

const int NUM_SETS = 10;
const int NUM_ALGOS = 4;
const char* ALGO_NAMES[NUM_ALGOS] = {"Quicksort", "Heapsort", "Mergesort", "sort (C++)"};

mt19937 rng(12345);

// ---------- QuickSort (pivot ngau nhien, de quy phan nho hon de tranh tran stack) ----------
void quickSort(vector<double>& a, int l, int r) {
    while (l < r) {
        double pivot = a[l + rng() % (r - l + 1)];
        int i = l, j = r;
        while (i <= j) {
            while (a[i] < pivot) i++;
            while (a[j] > pivot) j--;
            if (i <= j) {
                swap(a[i], a[j]);
                i++;
                j--;
            }
        }
        if (j - l < r - i) {
            quickSort(a, l, j);
            l = i;
        } else {
            quickSort(a, i, r);
            r = j;
        }
    }
}

void heap(vector<double>& a, int n, int i) {
    while (true) {
        int largest = i, l = 2 * i + 1, r = 2 * i + 2;
        if (l < n && a[l] > a[largest]) largest = l;
        if (r < n && a[r] > a[largest]) largest = r;
        if (largest == i) return;
        swap(a[i], a[largest]);
        i = largest;
    }
}

void heapSort(vector<double>& a) {
    int n = (int)a.size();
    for (int i = n / 2 - 1; i >= 0; i--) heap(a, n, i);
    for (int i = n - 1; i > 0; i--) {
        swap(a[0], a[i]);
        heap(a, i, 0);
    }
}

void mergeSortRec(vector<double>& a, vector<double>& tmp, int l, int r) {
    if (r - l < 1) return;
    int m = l + (r - l) / 2;
    mergeSortRec(a, tmp, l, m);
    mergeSortRec(a, tmp, m + 1, r);
    int i = l, j = m + 1, k = l;
    while (i <= m && j <= r) tmp[k++] = (a[i] <= a[j]) ? a[i++] : a[j++];
    while (i <= m) tmp[k++] = a[i++];
    while (j <= r) tmp[k++] = a[j++];
    for (k = l; k <= r; k++) a[k] = tmp[k];
}

void mergeSort(vector<double>& a) {
    vector<double> tmp(a.size());
    mergeSortRec(a, tmp, 0, (int)a.size() - 1);
}

bool readData(const string& path, vector<double>& a) {
    FILE* f = fopen(path.c_str(), "r");
    if (!f) return false;
    int n;
    if (fscanf(f, "%d", &n) != 1) { fclose(f); return false; }
    a.resize(n);
    for (int i = 0; i < n; i++) {
        if (fscanf(f, "%lf", &a[i]) != 1) { fclose(f); return false; }
    }
    fclose(f);
    return true;
}


double runAlgo(int algo, vector<double> a) { 
    auto start = steady_clock::now();
    switch (algo) {
        case 0: quickSort(a, 0, (int)a.size() - 1); break;
        case 1: heapSort(a); break;
        case 2: mergeSort(a); break;
        case 3: sort(a.begin(), a.end()); break;
    }
    auto end = steady_clock::now();
    if (!is_sorted(a.begin(), a.end())) {
        printf("  [LOI] %s sap xep sai!\n", ALGO_NAMES[algo]);
    }
    return duration<double, milli>(end - start).count();
}

int main(int argc, char* argv[]) {
    string dir = (argc > 1) ? argv[1] : DEFAULT_DATA_DIR;
    string csvPath = (argc > 2) ? argv[2] : DEFAULT_CSV;

    double times[NUM_SETS][NUM_ALGOS];

    for (int k = 1; k <= NUM_SETS; k++) {
        string path = dir + "/data" + to_string(k) + ".txt";
        vector<double> data;
        if (!readData(path, data)) {
            fprintf(stderr, "Khong doc duoc %s\n", path.c_str());
            return 1;
        }
        printf("Day %d (%d phan tu):\n", k, (int)data.size());
        for (int g = 0; g < NUM_ALGOS; g++) {
            times[k - 1][g] = runAlgo(g, data);
            printf("  %-11s %10.2f ms\n", ALGO_NAMES[g], times[k - 1][g]);
        }
    }

    FILE* out = fopen(csvPath.c_str(), "w");
    if (!out) {
        fprintf(stderr, "Khong ghi duoc %s\n", csvPath.c_str());
        return 1;
    }
    fprintf(out, "Du lieu");
    for (int g = 0; g < NUM_ALGOS; g++) fprintf(out, ",%s", ALGO_NAMES[g]);
    fprintf(out, "\n");
    for (int k = 0; k < NUM_SETS; k++) {
        fprintf(out, "%d", k + 1);
        for (int g = 0; g < NUM_ALGOS; g++) fprintf(out, ",%.2f", times[k][g]);
        fprintf(out, "\n");
    }
    fprintf(out, "Trung binh");
    for (int g = 0; g < NUM_ALGOS; g++) {
        double sum = 0;
        for (int k = 0; k < NUM_SETS; k++) sum += times[k][g];
        fprintf(out, ",%.2f", sum / NUM_SETS);
    }
    fprintf(out, "\n");
    fclose(out);

    printf("\nDa ghi ket qua vao %s\n", csvPath.c_str());
    return 0;
}
