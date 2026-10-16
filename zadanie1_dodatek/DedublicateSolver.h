#ifndef PWRRT_ZADANIA_DEDUBLICATESOLVER_H
#define PWRRT_ZADANIA_DEDUBLICATESOLVER_H
#include <unordered_map>
#include <vector>

#include "../Includes/sPoint.h"
#include "../Includes/DSU.h"

class DedublicateSolver {

    std::vector<sPoint> _points;
    DSU _dsu; // struktura zbiorow rozlacznych

    std::unordered_map<long long, std::vector<int>> _grid; // hash table

    long long hash_cell(long long cx, long long cy);

    void build_map_grid(int n, double d);
    void unite_close_points(int n, double d);

public:
    DedublicateSolver(std::vector<sPoint>& points);
    ~DedublicateSolver();

    void dedub(int n, double d);

    std::vector<sPoint> get_points();
};


#endif //PWRRT_ZADANIA_DEDUBLICATESOLVER_H
