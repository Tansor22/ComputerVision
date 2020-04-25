#ifndef IMAGETOPROCESS_H
#define IMAGETOPROCESS_H

#include <QPixmap>
#include <constants.h>
#include <dataretriver.h>
#include <helper.h>
#include <pointofinterest.h>



class ImageToProcess
{
public:
    ImageToProcess();
    ImageToProcess(QPixmap Pixmap, Canal type);
    double shiftError(int winSize, int x, int y, int dx, int dy);
    double getValueSafe(int x, int y);
    void setValueSafe(int x, int y, double value);
    void setDoubles(double* doubleData, int w, int h);
    QList<PointOfInterest> getPOIs(int winSize, bool isHarris = false);
    static QList<PointOfInterest> getPOIs(ImageToProcess* img, int winSize, bool isHarris = false);
    QList<PointOfInterest> filterPOIs(QList<PointOfInterest> pointsIn, int count);
    static QList<PointOfInterest> filterPOIs(ImageToProcess* img, QList<PointOfInterest> pointsIn, int count);

    // vars
    double* doubleData;
    int w;
    int h;


};

#endif // IMAGETOPROCESS_H
