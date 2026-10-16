#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cmath>

#include "SensorFusion.h"
#include "../Includes/sRecord.h"

bool read_csv(const std::string& file_name, std::vector<sRecord>& records) {
    std::ifstream file(file_name);
    if (!file.is_open()) {
        std::cerr << "Error opening file " << file_name << std::endl;
        return false;
    }

    std::string line;
    std::getline(file, line);

    while (std::getline(file, line)) {
        if (line.empty()) {
            continue;
        }

        std::istringstream ss(line);

        double t, x, y;
        char comma;

        ss >> t;
        ss >> comma >> x;
        ss >> comma >> y;
        records.push_back(sRecord(t, x, y));
    }

    file.close();
    return true;
}

// naprawia koordynaty y dla sensora 2
void fix_y_coordinates(std::vector<sRecord>& records) {
    for (auto &record : records) {
        record.y = record.y * -1;
    }
}

int main() {
    std::vector<sRecord> records_sens_1;
    std::vector<sRecord> records_sens_2;

    if (!read_csv("../sensor1.csv", records_sens_1)) {
        std::cerr << "Error opening file " << std::endl;
        return 1;
    }

    if (!read_csv("../sensor2.csv", records_sens_2)) {
        std::cerr << "Error opening file " << std::endl;
        return 1;
    }

    fix_y_coordinates(records_sens_2);

    // tablica bledow (roznic) pomiedzy sensorem 1 a sens 2
    std::vector<sRecord> err_table;
    create_err_vector(records_sens_1, records_sens_2, err_table);

    double T = 0;
    bool go=true;

    while (go) {
        std::cout << "Podaj T: ";
        std::cin >> T;

        if (T < 0 || T > 33) {
            std::cout << "wartosc spoza zakresu \n";
            continue;
        }
        else {
            // najblizsze pomiary sensora 1 wokol czasu T
            // oraz ich przyblizona pozycja w czasie T
            std::pair p = find_l_r(records_sens_1, T);
            std::pair p_n = interpolate(p.first, p.second, T);

            // najblizsze probki bledu wokol czasu T
            // przyblizony wektor bledu dla czasu T
            std::pair err_p = find_l_r(err_table, T);
            std::pair err_p_n = interpolate(err_p.first, err_p.second, T);

            // pomiar sensora 1 + blad w odniesieniu do sensora 2
            double final_x = p_n.first + err_p_n.first;
            double final_y = p_n.second + err_p_n.second;

            std::cout<< T << "," << final_x << "," << final_y << std::endl;

            char inp;

            while (true) {
                std::cout << "kontynuowac?: [t/n] ";
                std::cin >> inp;
                if (inp == 'n' || inp == 'N') {
                    go = false;
                    break;
                }
                if (inp == 't' || inp == 'T') {
                    break;
                }
            }
        }

    }

    return 0;
}