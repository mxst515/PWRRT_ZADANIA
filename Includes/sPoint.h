#ifndef PWRRT_ZADANIA_SPOINT_H
#define PWRRT_ZADANIA_SPOINT_H
#include <cmath>
#include <iostream>
#include <math.h>

struct sPoint {
    int id;
    double x;
    double y;

    sPoint(const int id_, const double x_, const double y_) : id(id_), x(x_), y(y_) {};

    void print_point() {
        std::cout << id << " " << x << " " << y;
    }

    std::string to_string() {
        return "CONE " + std::to_string(id) + " " + std::to_string(x) + " " + std::to_string(y);
    }

    double distance_to(const sPoint& p) {
        return (p.x - x) * (p.x - x) + (p.y - y) * (p.y - y);
    }
};

#endif //PWRRT_ZADANIA_SPOINT_H
