#ifndef PWRRT_ZADANIA_SENSORFUSION_H
#define PWRRT_ZADANIA_SENSORFUSION_H

#include <vector>
#include "../Includes/sRecord.h"

std::pair<sRecord, sRecord> find_l_r(std::vector<sRecord>& sens_rec, double t);
void create_err_vector(std::vector<sRecord>& sens_rec_1, std::vector<sRecord>& sens_rec_2, std::vector<sRecord>& err_vector);
std::pair<double, double> interpolate(sRecord& p1, sRecord& p2, double t);

#endif //PWRRT_ZADANIA_SENSORFUSION_H
