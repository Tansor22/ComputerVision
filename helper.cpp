#include "helper.h"

double Helper::normalizeStraight(int rgb) {
    return rgb / 255.0;
}
int Helper::normalizeReverse(double normalized) {
    return (int) (normalized * 255.0);
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
    double check = arr[rows * columns - 1];
    // int s = 0;
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
