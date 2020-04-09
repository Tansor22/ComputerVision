#ifndef DATARETRIVER_H
#include <QPixmap>
#include <QImage>
#include <constants.h>
#define DATARETRIVER_H


class DataRetriver
{
private:
    double (*mapper)(int);
    double map(int rgb);
public:
    DataRetriver();
    DataRetriver(double (*mapper)(int));
    // splits into canals
    double* retriveData(QImage qi, int canals);
    double* retriveData(int* arr, int w, int h, int canals);
    double* retriveData(QPixmap qp, int canals);
    // retriveng ints from Qt objects
    int* retriveData(QImage qi);
    int* retriveData(QPixmap qp);
};

#endif // DATARETRIVER_H
