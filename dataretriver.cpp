#include "dataretriver.h"

// retrives data from QPixmap, QImage, and canals value from rgb int array
double DataRetriver::map(int rgb) {
    return mapper != 0 ? mapper(rgb) : (double) rgb;
}
void DataRetriver::normalizeExtra(double* data) {

}

double* DataRetriver::retriveData(int* arr, int w, int h) {
    // checks for gray first
    int coef = 1;
    if ((GRAY & canals ) != GRAY) {
        // gray, coef = 0 will be used
        if ((R & canals ) == R) coef++;
        if ((G & canals ) == G) coef++;
        if ((B & canals ) == B) coef++;
        if ((A & canals ) == A) coef++;
    }

    double* data = new double[w * h * coef];
    for (int i = 0; i < h; i++)
        for (int j = 0; j < w; j++) {
            QRgb rgb = arr[i * w + j];
            if ((GRAY & canals ) == GRAY) {
                data[i * w + j] = map(qGray(rgb));
                continue;
            }
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

