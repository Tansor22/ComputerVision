#include "dataretriver.h"

// retrives data from QPixmap, QImage, and canals value from rgb int array
DataRetriver::DataRetriver()
{
    this->mapper = NULL;
}
DataRetriver::DataRetriver(double (*mapper)(int))
{
    this->mapper = mapper;
}
double DataRetriver::map(int rgb) {
    return mapper != NULL ? mapper(rgb) : (double) rgb;
}
// order is R G B, offset is w * h
double* DataRetriver::retriveData(QImage qi, int canals) {
    int coef = 0;
    if ((R & canals ) == R) coef++;
    if ((G & canals ) == G) coef++;
    if ((B & canals ) == B) coef++;
    if ((A & canals ) == A) coef++;

    int w = qi.width();
    int h = qi.height();
    double* data = new double[w * h * coef];
    for (int i = 0; i < h; i++)
        for (int j = 0; j < w; j++) {
            QRgb rgb = qi.pixel(j,i);
            if ((R & canals ) == R)
                data[i * w + j] = map(qRed(rgb));
            if ((G & canals ) == G)
                data[(i * w + j) + w * h] = map(qGreen(rgb));
            if ((B & canals ) == B)
                data[(i * w + j) + w * h * 2] = map(qBlue(rgb));
            if ((A & canals ) == A)
                data[(i * w + j) + w * h * 3] = map(qAlpha(rgb));
        }
    return data;
}
double* DataRetriver::retriveData(int* arr, int w, int h, int canals) {
    int coef = 0;
    if ((R & canals ) == R) coef++;
    if ((G & canals ) == G) coef++;
    if ((B & canals ) == B) coef++;
    if ((A & canals ) == A) coef++;

    double* data = new double[w * h * coef];
    for (int i = 0; i < h; i++)
        for (int j = 0; j < w; j++) {
            QRgb rgb = arr[i * w + j];
            if ((R & canals ) == R)
                data[i * w + j] = map(qRed(rgb));
            if ((G & canals ) == G)
                data[(i * w + j) + w * h] = map(qGreen(rgb));
            if ((B & canals ) == B)
                data[(i * w + j) + w * h * 2] = map(qBlue(rgb));
            if ((A & canals ) == A)
                data[(i * w + j) + w * h * 3] = map(qAlpha(rgb));
        }
    return data;
}
int* DataRetriver::retriveData(QImage qi) {
    int w = qi.width();
    int h = qi.height();
    int* data = new int[w * h];
    for (int i = 0; i < h; i++)
        for (int j = 0; j < w; j++)
            data[i * w + j] = qi.pixel(j,i);
    return data;
}
int* DataRetriver::retriveData(QPixmap qp) {
    return retriveData(qp.toImage());
}
double* DataRetriver::retriveData(QPixmap qp, int canals) {
    return retriveData(qp.toImage(), canals);
}
