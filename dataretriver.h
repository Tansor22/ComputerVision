#ifndef DATARETRIVER_H
#define DATARETRIVER_H
#include <QPixmap>
#include <QImage>
#include <QtDebug>
#include <constants.h>

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
    void normalizeExtra(int size, double* data);

};

#endif // DATARETRIVER_H
