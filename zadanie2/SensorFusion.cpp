#include "SensorFusion.h"
#include <algorithm>
#include <utility>
#include <vector>

/*
 * zwraca pair<sRecord, sRecord> [najblizszy pomiar na lewo od czasu t, najblizsy pomiar na prawo od czasu t]
 *
 * w przypadku gdy nie ma lewego, prawego pomiaru lub timestamp==t wystepuje w danych
 * zwracany jest ten sam pomiar .first == .second
 */
std::pair<sRecord, sRecord> find_l_r(std::vector<sRecord>& sens_rec, double t) {
    if (sens_rec.empty()) {
        return std::make_pair(sRecord(0,0,0), sRecord(0,0,0));
    }

    const auto it = std::lower_bound(sens_rec.begin(), sens_rec.end(), t, [](const sRecord& a, double t) {
        return a.timestamp < t;
    });

    if (it != sens_rec.end() && it->timestamp == t) {
        return std::make_pair(*it, *it);
    }

    if (it == sens_rec.begin()) {
        return std::make_pair(*it, *it);
    }

    if (it == sens_rec.end()) {
        return std::make_pair(*it, *it);
    }

    return std::make_pair(*(it-1), *it);
}


/*
 * zwraca pair<double,double> [interpolowany x, interpolowany y]
 *
 * In computer graphics it is common to linearly interpolate between an initial value v0
 * and an ending value v1 for a parameter t ranging from 0 to 1;
 *
 * https://en.wikipedia.org/wiki/Linear_interpolation
 */
std::pair<double, double> interpolate(sRecord& p1, sRecord& p2, double t) {
    double t1_l = p1.timestamp;
    double x1_l = p1.x;
    double y1_l = p1.y;

    double t1_r = p2.timestamp;
    double x1_r = p2.x;
    double y1_r = p2.y;

    double x1_interp, y1_interp;

    if (t1_l == t1_r) {
        x1_interp = x1_l;
        y1_interp = y1_l;
    }
    else {
        // parametr t [0,1] = ratio
        // (czas T - lewy.t)/(prawy.t - lewy.t)
        // przykladowo: jezeli czas T znajduje sie dokladnie pomiedzy pomiarami lewy i prawy
        // ratio = 0.5, wyliczony punkt (x,y) bedzie srednia arytmetyczna z dwoch recordow
        // im wieksza wartosc R - tym blizej prawego pomiaru znajdzie sie interpolowany punkt
        double ratio = (t - t1_l) / (t1_r - t1_l);

        x1_interp = x1_l + ratio * (x1_r - x1_l);
        y1_interp = y1_l + ratio * (y1_r - y1_l);
    }

    return std::make_pair(x1_interp, y1_interp);
}

/*
 * uzupelnia vector bledow
 * dla kazdego dostepnego pomiaru sensora 2 (t, x_s2, y_s2)
 * wylicza inteprolowana pozycje sensora 1 - dla dokladnie tego samego czasu t,
 * nastepnie wylicza delty przesuniecia punktu sensora 1, wzgledem sensora 2.
 *
 * gdy err_x = 0 i err_y = 0 - sensory zapisaly dla tego samego timestamp, dokladnie takie same pomiary x i y
 *
 * dodatkowo filtruje odstajace dane z zastosowaniem mediany i okna o wymiarze 3
 */
void create_err_vector(std::vector<sRecord>& sens_rec_1, std::vector<sRecord>& sens_rec_2, std::vector<sRecord>& err_vector) {
    std::vector<sRecord> raw_err;

    for (int i = 0; i < sens_rec_2.size(); i++) {
        double t = sens_rec_2[i].timestamp;
        double x_s2 = sens_rec_2[i].x;
        double y_s2 = sens_rec_2[i].y;

        std::pair p = find_l_r(sens_rec_1, t);

        std::pair inter_pair = interpolate(p.first, p.second, t);

        double x1_interp = inter_pair.first;
        double y1_interp = inter_pair.second;

        double err_x = x_s2 - x1_interp;
        double err_y = y_s2 - y1_interp;

        raw_err.push_back(sRecord(t, err_x, err_y));
    }

    err_vector.push_back(raw_err[0]);

    // filtrowanie danych
    for (size_t i = 1; i < raw_err.size() - 1; i++) {
        double x_vals[3] = {raw_err[i-1].x, raw_err[i].x, raw_err[i+1].x};
        double y_vals[3] = {raw_err[i-1].y, raw_err[i].y, raw_err[i+1].y};

        std::sort(x_vals, x_vals + 3);
        std::sort(y_vals, y_vals + 3);

        double med_x = x_vals[1];
        double med_y = y_vals[1];

        err_vector.emplace_back(raw_err[i].timestamp, med_x, med_y);
    }

    err_vector.push_back(raw_err.back());
}