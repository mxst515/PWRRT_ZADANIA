#include "PointsSolver.h"

#include <algorithm>
#include <complex>
#include <stack>

#include "../Includes/mMath.h"

PointsSolver::PointsSolver(std::vector<sPoint> points) : _points(points) {

}

PointsSolver::~PointsSolver() {}

std::vector<sPoint> PointsSolver::get_points() {
    return _points;
}

struct X_comp {
    bool operator()(const sPoint& a, const sPoint& b) const {
        if (a.x == b.x) {
            return a.y < b.y;
        }
        return a.x < b.x;
    }
};

struct AnglesComparator {
    sPoint p0;

    AnglesComparator(sPoint p0) : p0(p0) {}

    bool operator()(const sPoint& a, const sPoint& b) {
        double d = mMath::calc_det(this->p0, a, b);

        if (d == 0) {
            return this->p0.distance_to(a) < this->p0.distance_to(b);
        }

        return d > 0;
    }
}; //scomparator

void PointsSolver::dfs_rec(std::vector<sPoint>& sorted_pts, double d, int currInx, std::vector<bool>& visited, std::vector<sPoint>& curr_group) {
    visited[currInx] = true;
    curr_group.push_back(sorted_pts[currInx]);

    // przeszukiwanie w prawo
    for (int i = currInx+1; i < sorted_pts.size(); i++) {
        if (std::abs(sorted_pts[i].x - sorted_pts[currInx].x) > d) {
            break;
        }

        if (!visited[i] && sorted_pts[i].distance_to(sorted_pts[currInx]) < d*d) {
            dfs_rec(sorted_pts, d, i, visited, curr_group);
        }
    }
    // w lewo
    for (int i = currInx-1; i >= 0; i--) {
        if (std::abs(sorted_pts[i].x - sorted_pts[currInx].x) > d) {
            break;
        }

        if (!visited[i] && sorted_pts[i].distance_to(sorted_pts[currInx]) < d*d) {
            dfs_rec(sorted_pts, d, i, visited, curr_group);
        }
    }

}

void PointsSolver::dedublicate_points(double d) {

    std::vector<sPoint> sorted_points = _points;
    std::sort(sorted_points.begin(), sorted_points.end(), X_comp());

    std::vector<sPoint> new_points;

    int indx = sorted_points.size()+1;
    std::vector<bool> visited(_points.size(), false);

    for (int i=0; i<sorted_points.size(); i++) {
        if (!visited[i]) {
            std::vector<sPoint> group;
            dfs_rec(sorted_points, d, i, visited, group);

            if (group.size() > 1) {
                double sumx = 0, sumy = 0;
                for (auto & k : group) {
                    sumx += k.x;
                    sumy += k.y;
                }

                double ax = sumx / group.size();
                double ay = sumy / group.size();

                sPoint new_pt(indx++, ax, ay);
                new_points.push_back(new_pt);

                // wypisywanie nowych pkt
                std::cout << new_pt.to_string();

                std::cout << " [";
                for (int k = 0; k < group.size(); k++) {
                    if (k != 0) {
                        std::cout << ", ";
                    }
                    std::cout << group[k].id;
                }
                std::cout << "]" << std::endl;
                //
            }
            else {
                new_points.push_back(group[0]);
            }
        }
    }

    _points = new_points;
}

void PointsSolver::calculate_convexHull() {
    if (_points.empty()) {
        std::cout << "no points";
        return;
    }

    std::vector<sPoint> sorted_points = _points;

    int lowest_point_indx = find_min_y_indx(sorted_points);
    sPoint p0 = sorted_points[lowest_point_indx];

    sorted_points.erase(sorted_points.begin() + lowest_point_indx);

    sorted_points = filter_points(sorted_points, p0);


    if (sorted_points.size() < 3) {
        std::cout << "points < 3";
        return;
    }

    std::stack<sPoint> stack;

    stack.push(sorted_points[0]);
    stack.push(sorted_points[1]);
    stack.push(sorted_points[2]);

    for (int i = 3; i < sorted_points.size(); i++) {
        while (stack.size()>2 && mMath::calc_det(next_to_top(stack), stack.top(), sorted_points[i]) <= 0) {
            stack.pop();
        }
        stack.push(sorted_points[i]);
    }

    double obw = 0;
    sPoint pkt = stack.top();
    // std::cout << pkt.id << " ";
    stack.pop();
    sPoint curr = pkt;
    std::string final_string = "[" + std::to_string(pkt.id);

    while (!stack.empty()) {
        obw += sqrt(curr.distance_to(stack.top()));
        // std::cout << stack.top().id << " ";
        final_string += ", " + std::to_string(stack.top().id);
        curr = stack.top();
        stack.pop();
    }

    obw += sqrt(curr.distance_to(pkt));
    final_string += "]";

    std::cout << "ENCLOSURE " << obw << " " << final_string;

}

std::vector<sPoint> PointsSolver::filter_points(std::vector<sPoint> pts, sPoint& p0) {
    std::sort(pts.begin(), pts.end(), AnglesComparator(p0));
    std::vector<sPoint> result;
    result.push_back(p0);

    for (int i = 0; i < pts.size(); i++) {
        while (i<pts.size()-1 && mMath::calc_det(p0, pts[i], pts[i+1]) == 0) {
            i++;
        }
        result.push_back(pts[i]);
    }

    return result;
}

int PointsSolver::find_min_y_indx(std::vector<sPoint>& pts) {
    int inx = 0;
    sPoint min = pts[inx];

    for (int i = 1; i < pts.size(); i++) {
        if (pts[i].y < min.y || pts[i].y == min.y && pts[i].x < min.x) {
            min = pts[i];
            inx = i;
        }
    }

    return inx;
}

sPoint PointsSolver::next_to_top(std::stack<sPoint>& stack) {
    sPoint pt = stack.top();
    stack.pop();
    sPoint pntt = stack.top();
    stack.push(pt);
    return pntt;
}
