#ifndef HELPER_H
#include <constants.h>
#include <math.h>
#include <QDebug>
#define HELPER_H


class Helper
{
public:
    static void printAs2D(double *arr, int rows, int columns);
    static void printCanals(int *arr, int rows, int columns);
    static double normalizeStraight(int rgb);
    static int normalizeReverse(double normalized);
    static double* gauss(double sigma);
};

#endif // HELPER_H
