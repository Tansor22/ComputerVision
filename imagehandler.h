#ifndef IMAGEHANDLER_H
#define IMAGEHANDLER_H

#include "imagetoprocess.h"
#include <QImage>
#include <QPixmap>
#include <QPainter>



class ImageHandler
{

public:
    ImageHandler();
    static QImage join(ImageToProcess o1, ImageToProcess o2);
    static void markWithWhite(ImageToProcess &itp, QList<PointOfInterest> pois, int crossSize = 3);
    static void markWithRGB(ImageToProcess &itp, QList<PointOfInterest> pois, QRgb rgb, int crossSize = 3);
};

#endif // IMAGEHANDLER_H
