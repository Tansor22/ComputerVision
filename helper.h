#ifndef HELPER_H
#include <constants.h>
#include <math.h>
#include <QDebug>
#define HELPER_H


class Helper
{
private:
    static const int ELEMS_IN_LINE = 6;
public:
    static void printAs2D(double *arr, int rows, int columns);
    static void printSample(int from, int to, double* data);
    static void printSample(int sampleSize, double* data, int srcSize);
    static void printCanals(int *arr, int rows, int columns);
    static double normalizeStraight(int rgb);
    static int normalizeReverse(double normalized);
    static double* gauss(double sigma);
    static bool isGray(Canal type) {return (GRAY & type ) == GRAY; }
    static double* sample(int from, int to, double* data);
    static double* sample(int sampleSize, double* data, int srcSize);
};

#endif // HELPER_H
