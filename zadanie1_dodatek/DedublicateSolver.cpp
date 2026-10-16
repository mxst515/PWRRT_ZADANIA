#include "DedublicateSolver.h"

DedublicateSolver::DedublicateSolver(std::vector<sPoint>& points) : _points(points) {

}

DedublicateSolver::~DedublicateSolver() {}

std::vector<sPoint> DedublicateSolver::get_points() {
    return _points;
}

// funkcja hashujaca
long long DedublicateSolver::hash_cell(long long cx, long long cy) {
    return cx * 73856093LL + cy * 19349663LL;
}

/*
 * buduje mape/grid o wielkosci cell = d
 *
 * dla kazdego punktu wylicza dla niego komorke [cx,cy] na gridzie
 * nastepnie dodaje pod wyliczony klucz jego index
 */
void DedublicateSolver::build_map_grid(int n, double d) {
    for (int i=0; i<n; i++) {
        int cx = (int)std::floor(_points[i].x/d);
        int cy = (int)std::floor(_points[i].y/d);
        _grid[hash_cell(cx, cy)].push_back(i);
    }
}

/*
 * wyszukuje punkty lezace blisko siebie i laczy je w jedna grupe
 *
 * zachowuje przechodniosc p1->p2, p2->p3, p1->p3
 *
 * przechodzi przez wszystkie punkty na gridzie, i przeszukuje sasiadujace kratki
 * np.
 * [ ][*][ ]
 * [ ][*][*]
 * [ ][ ][ ] //
 *
 * jezeli dwa punkty leza obok siebie w odleglosci < d
 * sa scalane pod jedna strukture (dsu.union)
 *
 */
void DedublicateSolver::unite_close_points(int n, double d) {
    for (int i=0; i<n; i++) {
        // [cx, cy] na gridzie
        int cx = (int)std::floor(_points[i].x/d);
        int cy = (int)std::floor(_points[i].y/d);

        for (int dx=-1; dx<=1; dx++) {
            for (int dy=-1; dy<=1; dy++) {
                auto it = _grid.find(hash_cell(cx+dx, cy+dy));
                if (it == _grid.end()) {continue;} // gdy kratka na gridzie jest pusta, zostaje pominieta

                // sprawdzanie punktow znajdujacych sie w komorce i porownywanie z naszym aktualnym
                for (int j : it->second) {
                    if (j<=i) {continue;}
                    if (_points[i].distance_to(_points[j]) <= d*d) {
                        _dsu.unite(i, j);
                    }
                }
            }
        }
    }
}

/*
 * przeprowadza dedublikacje punktow, wypisuje nowe oraz z jakich zostaly utworzone
 *
 */
void DedublicateSolver::dedub(int n, double d) {
    if (_points.empty()) {return;}
    _dsu.reset(n);

    int new_indx = _points.size()+1;

    build_map_grid(n, d);

    unite_close_points(n, d);

    std::unordered_map<int, std::vector<sPoint>> point_groups; // <root dsu, punkty pod tym samym root>

    // dodaje punkty do kazdej z poszczegolnych grup (jezeli punkt nie ma nikogo w odleglosci < d)
    // pozostaje sam w grupie
    for (int i=0; i<n; i++) {
        int root_id = _dsu.find(i);
        point_groups[root_id].push_back(_points[i]);
    }

    std::vector<sPoint> result;
    for (auto& g_pair : point_groups) {
        auto& group = g_pair.second;

        if (group.empty()) {continue;}
        if (group.size() > 1) {
            double sumx = 0, sumy = 0;
            for (auto& p : group) {
                sumx += p.x;
                sumy += p.y;
            }

            sPoint new_p(new_indx++, sumx/group.size(), sumy/group.size());

            // wypisywanie punktow
            std::cout << new_p.to_string();
            std::cout << " [";
            for (int k = 0; k < group.size(); k++) {
                if (k != 0) {
                    std::cout << ", ";
                }
                std::cout << group[k].id;
            }
            std::cout << "]" << std::endl;

            result.push_back(new_p);
        }
        else {
            result.push_back(group[0]);
        }

    }

    _points = result;
}
