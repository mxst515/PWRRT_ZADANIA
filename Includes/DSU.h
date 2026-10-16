#ifndef PWRRT_ZADANIA_DSU_H
#define PWRRT_ZADANIA_DSU_H
#include <numeric>
#include <vector>

class DSU {
    std::vector<int> parent;

public:
    DSU() {};
    DSU(int n) {
        reset(n);
    }


    void reset(int n) {
        parent.resize(n);
        // kazdy wezel na starcie jest swoim wlasnym rodzicem
        for (int i = 0; i < n; i++) {
            parent[i] = i;
        }
    }

    int find(int x) {
        while (parent[x] != x) {
            parent[x] = find(parent[x]);
            x = parent[x];
        }
        return x;
    }

    void unite(int a, int b) {
        parent[find(a)] = find(b);
    }
};

#endif //PWRRT_ZADANIA_DSU_H
