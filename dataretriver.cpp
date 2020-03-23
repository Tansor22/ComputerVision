#include "dataretriver.h"

// retrives data from QPixmap, QImage, and canals value from rgb int array
DataRetriver::DataRetriver()
{

}
// order is R G B, offset is w * h
int* DataRetriver::retriveData(QImage qi, int canals) {
    int coef = 0;
    if ((R & canals ) == R) coef++;
    if ((G & canals ) == G) coef++;
    if ((B & canals ) == B) coef++;

    int w = qi.width();
    int h = qi.height();
    int* data = new int[w * h * coef];
    for (int i = 0; i < h; i++)
        for (int j = 0; j < w; j++) {
            QRgb rgb = qi.pixel(j,i);
            if ((R & canals ) == R)
                data[i * w + j] = qRed(rgb);
            if ((G & canals ) == G)
                data[(i * w + j) + w * h] = qGreen(rgb);
            if ((B & canals ) == B)
                data[(i * w + j) + w * h * 2] = qBlue(rgb);
        }
    return data;
}
int* DataRetriver::retriveData(int* arr, int w, int h, int canals) {
    int coef = 0;
    if ((R & canals ) == R) coef++;
    if ((G & canals ) == G) coef++;
    if ((B & canals ) == B) coef++;

    int* data = new int[w * h * coef];
    for (int i = 0; i < h; i++)
        for (int j = 0; j < w; j++) {
            QRgb rgb = arr[i * w + j];
            if ((R & canals ) == R)
                data[i * w + j] = qRed(rgb);
            if ((G & canals ) == G)
                data[(i * w + j) + w * h] = qGreen(rgb);
            if ((B & canals ) == B)
                data[(i * w + j) + w * h * 2] = qBlue(rgb);
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
int* DataRetriver::retriveData(QPixmap qp, int canals) {
    return retriveData(qp.toImage(), canals);
}
