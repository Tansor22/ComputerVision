#ifndef DATARETRIVER_H
#include <QPixmap>
#include <QImage>
#include <constants.h>
#define DATARETRIVER_H


class DataRetriver
{
private:
    double (*mapper)(int);
    // canals retriver works with
    Canal canals;
    double map(int rgb);
public:
    DataRetriver(Canal canals = 0, double (*mapper)(int) = 0)
        : mapper(mapper), canals(canals) {};
    // splits into canals
    double* retriveData(int* arr, int w, int h);
    // retriveng ints from Qt objects
    int* retriveData(QImage qi);
    int* retriveData(QPixmap qp);
    // normalizes pixels
    void normalizeExtra(double* data);

};

#endif // DATARETRIVER_H
