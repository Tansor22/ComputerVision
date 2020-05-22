#ifndef IMAGETOPROCESS_H
#define IMAGETOPROCESS_H
#include <QPixmap>
#include <constants.h>
#include <dataretriver.h>
#include <helper.h>
#include <pointofinterest.h>
#include "poishandler.h"
#include "sequentialconvolutionaltool.h"
#include "parallelconvolutionaltool.h"


class ImageToProcess
{

public:
    enum OutOfBoundPolicy {
        EDGE,
        BLACK,
        WHITE,
        SPREAD
    };
    ImageToProcess();
    ~ImageToProcess();
    ImageToProcess(Canal type, double* data, int w, int h);
    ImageToProcess(QPixmap Pixmap, Canal type);
    ImageToProcess(ImageToProcess *itp, OutOfBoundPolicy = EDGE);
    ImageToProcess(Canal type, int w, int h);
    double shiftError(int winSize, int x, int y, int dx, int dy);
    double getValueSafe(int x, int y);
    double operator[](int i) const;
    void setValueSafe(int x, int y, double value);
    void setValueSafe(int x, int y, QRgb rgb);
    void setDoubles(Canal type, double* doubleData, int w, int h);
    int* toIntRGB();
    QImage toQImage();
    void derivativeX();
    void save(QString fileName);
    void cross(double* kernel, int kernelW, int kernelH, double divider = 1.0);
    void gaussBlur(double sigma);
    void derivativeY();
    void gradient();
    QList<PointOfInterest> getPOIs(int winSize, bool isHarris = false);
    QList<PointOfInterest> filterPOIs(QList<PointOfInterest> pointsIn, int count);
    void downsample();

    // vars
    double* doubleData;
    int w = 0;
    int h = 0;
    Canal type;
    OutOfBoundPolicy oobp = EDGE;
    QString name;


    int getW() const;
    int getH() const;
    double *getDoubles() const;
    Canal getType() const;
    void setType(const Canal &value);
    void setName(const QString &value);
};

#endif // IMAGETOPROCESS_H
