#include <algorithm>
#include <cmath>
#include <cstdio>
#include <functional>
#include <random>
#include <string>
#include <vector>
using namespace std;

const int N = 1000000;        
const int NUM_SETS = 10;      
const unsigned SEED = 20261001; 

int main(int argc, char* argv[]) {
    string dir = (argc > 1) ? argv[1] : "data";

    mt19937 rng(SEED);
    uniform_real_distribution<double> dist(0.0, 1000000.0);

    for (int k = 1; k <= NUM_SETS; k++) {
        vector<double> a(N);
        for (auto& x : a) {

            x = round(dist(rng) * 1000.0) / 1000.0;
        }

        if (k == 1) sort(a.begin(), a.end());                 
        else if (k == 2) sort(a.begin(), a.end(), greater<double>()); 

        string path = dir + "/data" + to_string(k) + ".txt";
        FILE* f = fopen(path.c_str(), "w");
        if (!f) {
            fprintf(stderr, "Khong mo duoc file %s (thu muc co ton tai khong?)\n", path.c_str());
            return 1;
        }
        fprintf(f, "%d\n", N);
        for (int i = 0; i < N; i++) fprintf(f, "%.3f\n", a[i]);
        fclose(f);
        printf("Da ghi %s\n", path.c_str());
    }
    return 0;
}
