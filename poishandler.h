#ifndef POISHANDLER_H

#define POISHANDLER_H

#include "pointofinterest.h"


// circular dependency resolving
class ImageToProcess;
class PoisHandler
{
public:
    enum PoisAlgo {
        MORAVEC, HARRIS
    };
    PoisHandler();
    static QList<PointOfInterest> filterPOIs(int w, int h, QList<PointOfInterest> pointsIn, int count);
    static QList<PointOfInterest> getPOIs(ImageToProcess img, int winSize, bool isHarris = false);
    static QList<PointOfInterest> handle(PoisAlgo algo, ImageToProcess itp, int winSize, int nPoints, bool needDescriptors = false);
    static QList<PointOfInterest> handleMoravec(ImageToProcess itp, int winSize, int nPoints, bool needDescriptors);
    static QList<PointOfInterest> handleHarris(ImageToProcess itp, int winSize, int nPoints, bool needDescriptors);
};

#endif // POISHANDLER_H
