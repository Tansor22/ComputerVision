#ifndef HELPER_H
#include <constants.h>
#include <math.h>
#include <QPixmap>
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
    static double* copyOf(double* arr, int size);
    static double normalizeStraight(int rgb);
    static double distance(double x1, double x2, double y1, double y2);
    static QList<double> wrapInQList(double* arr, int size);
    static QList<QList<double>> wrapInQListPerCanal(Canal type, double* arr, int w, int h);
    static int* toIntRGB(Canal type, double* data, int size);
    static int normalizeReverse(double normalized);
    static double* gauss(double sigma);
    static bool isGray(Canal type) {return (GRAY & type ) == GRAY; }
    static int canalsCount(Canal type) {return isGray(type) ? 1 : noAlpha(type) ? 3 : 4;}
    static bool noAlpha(Canal type) {return (A & type ) != A; }
    static double* sample(int from, int to, double* data);
    static double* sample(int sampleSize, double* data, int srcSize);
    static QString asQStringWithPrecision(double num, int precision) {return QString::number(num, 'f', precision);}
    static double* getZeroFilledArr(int size);
    // templates
    template <typename T>
    // & means it will be mutated
    static void init(QList<T> &list, int size) {
        for(int i = 0; i < size; i++) list << 0;
    };
};

#endif // HELPER_H
