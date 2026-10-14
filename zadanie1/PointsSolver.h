#ifndef PWRRT_ZADANIA_POINTSSOLVER_H
#define PWRRT_ZADANIA_POINTSSOLVER_H

#include <stack>
#include <vector>
#include "../Includes/sPoint.h"

class PointsSolver {
    std::vector<sPoint> _points;

    void dfs_rec(std::vector<sPoint>& sorted_pts, double d, int currInx, std::vector<bool>& visited, std::vector<sPoint>& group);

    sPoint next_to_top(std::stack<sPoint>& stack);
    std::vector<sPoint> filter_points(std::vector<sPoint> points, sPoint& p0);
    int find_min_y_indx(std::vector<sPoint>& pts);

public:
    PointsSolver(std::vector<sPoint> points);
    ~PointsSolver();

    void dedublicate_points(double d);


    void calculate_convexHull();


    std::vector<sPoint> get_points();
};


#endif //PWRRT_ZADANIA_POINTSSOLVER_H
