#include "dataretriver.h"

// retrives data from QPixmap, QImage, and canals value from rgb int array
double DataRetriver::map(int rgb) {
    return mapper != 0 ? mapper(rgb) : (double) rgb;
}
void DataRetriver::normalizeExtra(int size, double* data) {
    int canalI = 0;
    // gray
    double max;
    double min;
    if ((GRAY & canals ) == GRAY)  {
        max = data[0];
        min = data[0];
    }
    // R
    double maxR;
    double minR;
    if ((R & canals ) == R)  {
        maxR = data[size * canalI];
        minR = data[size * canalI];
        canalI++;
    }
    // G
    double maxG;
    double minG;
    if ((G & canals ) == G)  {
        maxG = data[size * canalI];
        minG = data[size * canalI];
        canalI++;
    }
    // B
    double maxB;
    double minB;
    if ((B & canals ) == B)  {
        maxB = data[size * canalI];
        minB = data[size * canalI];
        canalI++;
    }
    // A
    double maxA;
    double minA;
    if ((A & canals ) == A)  {
        maxA = data[size * canalI];
        minA = data[size * canalI];
    }
    // reset canal pointer
    canalI = 0;

    int i, target;
    // searching for max and min values
    for (i = 0; i < size; i++) {
        // GRAYSCALED IMAGE
        if ((GRAY & canals ) == GRAY)  {
            if (data[i] < min) min = data[i];
            if (data[i] > max) max = data[i];
            continue;
        }
        // RGB COLOR SCHEME
        if ((R & canals ) == R) target = i + size * canalI++;
        if (data[target] < minR) minR = data[target];
        if (data[target] > maxR) maxR = data[target];

        if ((G & canals ) == G) target = i + size * canalI++;
        if (data[target] < minG) minG = data[target];
        if (data[target] > maxG) maxG = data[target];

        if ((B & canals ) == B) target = i + size * canalI++;
        if (data[target] < minB) minB = data[target];
        if (data[target] > maxB) maxB = data[target];

        if ((A & canals ) == A) target = i + size * canalI;
        if (data[target] < minA) minA = data[target];
        if (data[target] > maxA) maxA = data[target];

        // reset canal pointer
        canalI = 0;
    }

    // normalizing
    for (i = 0; i < size; i++) {
        // GRAYSCALED IMAGE
        if ((GRAY & canals ) == GRAY)  {
            // (val - min) * ((globalMax - globalMin) / (max - min)) + globalMin;
            data[i] = (data[i] - min) * (1.0 / (max - min));
            continue;
        }
        // RGB COLOR SCHEME
        if ((R & canals ) == R) {
            target = i + size * canalI++;
            data[target] = (data[target] - minR) * (1.0 / (maxR - minR));
        }

        if ((G & canals ) == G) {
            target = i + size * canalI++;
            data[target] = (data[target] - minR) * (1.0 / (maxR - minR));
        }

        if ((B & canals ) == B) {
            target = i + size * canalI++;
            data[target] = (data[target] - minR) * (1.0 / (maxR - minR));
        }
        if ((A & canals ) == A) {
            target = i + size * canalI;
            data[target] = (data[target] - minR) * (1.0 / (maxR - minR));
        }

        // reset canal pointer
        canalI = 0;
    }

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
            // the idea
            int canalI = 0;
            if ((R & canals ) == R)
                data[i * w + j + w * h * canalI++] = map(qRed(rgb));
            if ((G & canals ) == G)
                data[(i * w + j) + w * h * canalI++] = map(qGreen(rgb));
            if ((B & canals ) == B)
                data[(i * w + j) + w * h * canalI++] = map(qBlue(rgb));
            if ((A & canals ) == A)
                data[(i * w + j) + w * h * canalI] = map(qAlpha(rgb));

            // reset canal
            canalI = 0;
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

