#ifndef DATARETRIVER_H
#include <QPixmap>
#include <QImage>
#include <constants.h>
#define DATARETRIVER_H


class DataRetriver
{
public:
    DataRetriver();
    int* retriveData(QImage qi, int canals);
    int* retriveData(int* arr, int w, int h, int canals);
    int* retriveData(QImage qi);
    int* retriveData(QPixmap qp);
    int* retriveData(QPixmap qp, int canals);
};

#endif // DATARETRIVER_H
