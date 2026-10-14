#include <iostream>
#include <fstream>
#include <vector>

#include "PointsSolver.h"
#include "../Includes/sPoint.h"


bool read_file_and_make_points(const std::string& file_name, int& n, double&d, std::vector<sPoint>& points) {
    std::ifstream file(file_name);
    if (!file.is_open()) {
        std::cerr << "Error opening file " << file_name << std::endl;
        return false;
    }

    file >> n >> d;

    for (int i = 0; i < n; i++) {
        int id;
        double x, y;
        file >> id;
        file >> x >> y;
        points.push_back(sPoint(id, x, y));
    }

    file.close();
    return true;
}

int main() {
    std::string file_path;
    std::cout<<"Podaj sciezke do pliku: ";
    std::cin>>file_path;

    std::vector<sPoint> points;
    int n;
    double d;

    if (!read_file_and_make_points(file_path, n, d, points)) {
        return 1;
    }

    PointsSolver point_solver(points);

    point_solver.dedublicate_points(d);

    point_solver.calculate_convexHull();

    return 0;
}
