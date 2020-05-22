#ifndef DATARETRIEVER_H
#define DATARETRIEVER_H
#include <QPixmap>
#include <QImage>
#include <QtDebug>
#include <constants.h>

class DataRetriever
{
private:
    double (*mapper)(QRgb);
    // canals retriever works with
    Canal canals;
    double map(QRgb rgb);
public:
    DataRetriever(Canal canals = 0, double (*mapper)(QRgb) = 0)
        : mapper(mapper), canals(canals) {};
    // splits into canals
    double* retrieveData(QRgb* arr, int w, int h);
    // retrieving ints from Qt objects
    QRgb* retrieveData(QImage qi);
    QRgb* retrieveData(QPixmap qp);
    // normalizes pixels
    void normalizeExtra(int size, double* data);

};

#endif // DATARETRIEVER_H
