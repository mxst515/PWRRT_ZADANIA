#ifndef PWRRT_ZADANIA_SRECORD_H
#define PWRRT_ZADANIA_SRECORD_H

struct sRecord {
    double timestamp;
    double x;
    double y;

    sRecord(double t_, double x_, double y_) : timestamp(t_), x(x_), y(y_) {};

};

#endif //PWRRT_ZADANIA_SRECORD_H
