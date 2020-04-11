#include "helper.h"

double Helper::normalizeStraight(int rgb) {
    return rgb / 255.0;
}
int Helper::normalizeReverse(double normalized) {
    return (int) (normalized * 255.0);
}
double* Helper::sample(int from, int to, double *data) {
    // non negative
    assert(from >= 0 && to >= 0);
    int size = to - from;
    // to is greater than from
    assert(size > 0);
    double* sample = new double[size];
    for (int i = from; i < to; i++)
        sample[i] = data[i];
    return sample;
}
double* Helper::sample(int sampleSize, double *data, int srcSize) {
    // 0 .. (srcSize - sampleSize)
    int from = rand() & (srcSize - sampleSize);
    int to = from + sampleSize;
    return sample(from, to, data);
}
void Helper::printSample(int from, int to, double *data) {
    double* toPrint = sample(from, to, data);
    std::string str = "";
    int elems = 0;

    for (int i = 0; i < to - from; i++) {
        str += std::to_string(toPrint[i]) + " ";
        if (++elems > ELEMS_IN_LINE) { str += "\t\n"; elems = 0;};
    }
    qDebug() << str.c_str() << endl;
}
void Helper::printSample(int sampleSize, double *data, int srcSize) {
    double* toPrint = sample(sampleSize, data, srcSize);
    std::string str = "";
    int elems = 0;

    for (int i = 0; i < sampleSize; i++) {
        str += std::to_string(toPrint[i]) + " ";
        if (++elems > ELEMS_IN_LINE) { str += "\t\n"; elems = 0;};
    }
    qDebug() << str.c_str() << endl;
}

double* Helper::gauss(double sigma) {
    int size = floor(3 * sigma);
    double *matrix_gauss = new double[size * size];
    int halfSize = size / 2;
    double ss2 = 2 * sigma * sigma;
    double firstDrob = 1.0 / (M_PI * ss2);
    double test_sum = 0.0;
    int rowI = 0;
    for(int x = -halfSize ; x <= halfSize; x++){
        int columnI = 0;
        for(int y = -halfSize; y <= halfSize ; y++){
            double gauss = firstDrob * exp( -(x * x + y * y) / ss2);
            matrix_gauss[rowI * size + columnI++] = gauss;
            test_sum += gauss;
        }
        rowI++;
    }
    for(int x = -halfSize ; x <= halfSize; x++)
        for(int y = -halfSize; y <= halfSize ; y++)
            matrix_gauss[(x + halfSize) * size + y + halfSize] /= test_sum;

    Helper::printAs2D(matrix_gauss, size, size);
    return matrix_gauss;
}
void Helper::printAs2D(double *arr, int rows, int columns) {
    std::string str = "";
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            str += std::to_string(arr[i * columns + j]) + " ";

        }
        str += "\t\n";

    }
    qDebug() << str.c_str() << endl;
}
void Helper::printCanals(int *arr, int rows, int columns) {
    std::string str = "";
    for (int c = 0; c < 3; c++){
        str += "Canal " + std::to_string(c + 1) + "\t\n";
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < columns; j++) {
                str += std::to_string(arr[(i * columns + j) + c * rows * columns]) + " ";

            }
            str += "\t\n";
        }
    }
    qDebug() << str.c_str() << endl;
}
