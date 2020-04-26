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
    void setDoubles(Canal type, double* doubleData, int w, int h);
    int* toIntRGB();
    QList<PointOfInterest> getPOIs(int winSize, bool isHarris = false);
    static QList<PointOfInterest> getPOIs(ImageToProcess* img, int winSize, bool isHarris = false);
    QList<PointOfInterest> filterPOIs(QList<PointOfInterest> pointsIn, int count);
    void downsample();
    static QList<PointOfInterest> filterPOIs(int w, int h, QList<PointOfInterest> pointsIn, int count);

    // vars
    double* doubleData;
    Canal type;
    int w;
    int h;


};

#endif // IMAGETOPROCESS_H
