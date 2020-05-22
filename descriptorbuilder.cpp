#include "descriptorbuilder.h"

DescriptorBuilder::DescriptorBuilder(ImageToProcess *itp, int nBaskets, int nHistograms, int gridSize)
    : nBaskets(nBaskets), nHistograms(nHistograms), gridSize(gridSize), w(itp->getW()), h(itp->getH()) {

    // searching for gradient
    gradient = new ImageToProcess(itp, ImageToProcess::BLACK);
    gradient->gradient();

    // searching for derivatives
    dx = new ImageToProcess(itp);
    dy = new ImageToProcess(itp);
    dx->derivativeX();
    dy->derivativeY();

    // searching for gradient direction
    gradientDirection = new ImageToProcess(itp);
    for (int j = 0; j < h; j++) {
        for (int i = 0; i < w; i++) {
            double gradDir = atan2(dy->getValueSafe(i, j),
                                   dx->getValueSafe(i, j)) * 180.0 / M_PI + 180.0;  // magic
            gradientDirection->setValueSafe(i, j, gradDir);
        }
    }

    //Sandbox::write(*gradientDirection, "gradientDirection");

    double sigma = static_cast<double>(gridSize) / 6;
    int halfSize = gridSize / 2;
    gaussKernel = new double[(gridSize + 1) * (gridSize + 1)];

    double ss2 = sigma * sigma;
    double c = 1 / (2 * M_PI * ss2);
    double divider = 2 * ss2;

    for (int u = -halfSize; u <= halfSize; u++)
        for (int v = -halfSize; v <= halfSize; v++)
            gaussKernel[(u + halfSize) * gridSize + (v  + halfSize)] = c * exp(- (u * u + v * v) / divider);
}

Descriptor DescriptorBuilder::buildDescriptor(PointOfInterest inputPoint)
{
    Descriptor descriptor (nBaskets, nHistograms);
    double basketSize = 360.0 / nBaskets;
    int descriptorRadius = gridSize / 2;

    for(int i = -descriptorRadius; i < descriptorRadius; i++){
        for(int j = -descriptorRadius; j < descriptorRadius; j++){

           int angledX = static_cast<int>
                    (i * cos((360.0 - inputPoint.getAngle()) * M_PI / 180.0)
                      - j * sin((360.0 - inputPoint.getAngle()) * M_PI / 180.0)
                     + 0.5);

            int angledY = static_cast<int>
                    (i * sin((360.0 - inputPoint.getAngle()) * M_PI / 180.0)
                     + j * cos((360.0 - inputPoint.getAngle()) * M_PI / 180.0)
                     + 0.5);


            double angle = atan2(angledY, angledX );
            angledY -= static_cast<int>(sin(angle) * qMax(angledX - 8, 0) + 0.5);
            angledX -= static_cast<int>(cos(angle) * qMax(angledY - 8, 0) + 0.5);

                // in range
                if(angledX < -descriptorRadius) {
                    angledX = 0;
                }
                else {
                    if(angledX >= descriptorRadius) {
                        angledX = descriptorRadius;
                    }
                    else {
                        angledX += descriptorRadius;
                    }
                }

                if(angledY < -descriptorRadius) {
                    angledY = 0;
                }
                else {
                    if(angledY >= descriptorRadius) {
                        angledY = descriptorRadius;
                    }
                    else {
                        angledY += descriptorRadius;
                    }
                }


                double localDirection =
                        gradientDirection->getValueSafe(inputPoint.getX() + i,
                                                        inputPoint.getY() + j)
                        - inputPoint.getAngle();
                // to 0..360 range
                localDirection = (localDirection < 0) ? localDirection + 360 : localDirection;
                localDirection = (localDirection > 360) ? localDirection - 360 : localDirection;

                int x = static_cast<int>(static_cast<double>(angledX)
                                         / (static_cast<double>(descriptorRadius * 2) / nHistograms) - 0.2);
                int y = static_cast<int>(static_cast<double>(angledY)
                                         / (static_cast<double>(descriptorRadius * 2) / nHistograms) - 0.2);


                int histogramNumber = y * nHistograms + x;

                int basketNumber = static_cast<int>(localDirection / basketSize + 0.5);
                if (basketNumber == nBaskets)
                    basketNumber--;

                double localBasketCenter = static_cast<double>(basketNumber * basketSize) + static_cast<double>(basketSize) / 2.0;

                int relatedBasket;
                if(localDirection < localBasketCenter)
                    relatedBasket = (basketNumber - 1) == -1 ? nBaskets - 1 : basketNumber - 1;
                else
                    relatedBasket = (basketNumber + 1) == nBaskets ? 0 : basketNumber + 1;

                double currentCenterDistance = abs(localBasketCenter - localDirection);
                double relatedCenterDistance = basketSize - currentCenterDistance;


                descriptor.setBasket(histogramNumber, basketNumber, descriptor.getBasket(histogramNumber, basketNumber)
                                     + gradient->getValueSafe(inputPoint.getX() + i, inputPoint.getY() + j)
                                     * (1 - currentCenterDistance / basketSize));

                descriptor.setBasket(histogramNumber, relatedBasket, descriptor.getBasket(histogramNumber, relatedBasket)
                                     + gradient->getValueSafe(inputPoint.getX() + i, inputPoint.getY() + j)
                                     * (1 - relatedCenterDistance / basketSize));

        }
    }
    descriptor.normalize();
    return descriptor;

}

QList<PointOfInterest> DescriptorBuilder::calcOrientPoints(QList<PointOfInterest> points)
{
    QList<PointOfInterest> orientPoints;

    int localNBaskets = 36;
    double localBasketSize = 360.0 / localNBaskets;
    int descriptorRadius = gridSize / 2;

    for(int index = 0; index < points.size(); index++) {

        double localBaskets[localNBaskets];
        for (int i = 0; i < localNBaskets; i++){
            localBaskets[i] = 0;
        }

        for(int i = -descriptorRadius; i <= descriptorRadius; i++){
            for(int j = -descriptorRadius; j <= descriptorRadius; j++){

                // in range
                if(Helper::distance(static_cast<double>(i), static_cast<double>(j), 0.0,0.0)
                        < sqrt(pow(descriptorRadius, 2) + pow(descriptorRadius,2))){

                    double direction =  gradientDirection->getValueSafe(points[index].getX() + i, points[index].getY() + j);

                    int basketNumber = static_cast<int> (direction / localBasketSize + 0.5);

                    double localBasketCenter = static_cast<double>(basketNumber * localBasketSize) + localBasketSize / 2.0;

                    int relatedBasketNumber;
                    if(direction < localBasketCenter)
                        relatedBasketNumber = basketNumber - 1;
                    else
                        relatedBasketNumber = basketNumber + 1;

                    if (relatedBasketNumber < 0)
                        relatedBasketNumber = localNBaskets - 1;
                    if (relatedBasketNumber > localNBaskets - 1)
                        relatedBasketNumber = 0;


                    double thisCenterDistance = abs(localBasketCenter - direction);
                    double relatedCenterDistance = localBasketSize - thisCenterDistance;


                    localBaskets[basketNumber] += gradient->getValueSafe(points[index].getX() + i,
                                                                         points[index].getY() + j)
                            * (1 - thisCenterDistance / localBasketSize)
                            * gaussKernel[(i + descriptorRadius) * gridSize + (j  + descriptorRadius)];


                    localBaskets[relatedBasketNumber] += gradient->getValueSafe(points[index].getX() + i,
                                                                                points[index].getY() + j)
                                                         * (1 - relatedCenterDistance / localBasketSize)
                                                         * gaussKernel[(i + descriptorRadius) * gridSize + (j  + descriptorRadius)];
                }
            }
        }

        double firstMax = -1;
        int firstMaxI = -1;
        double secondMax = -1;
        int secondMaxI = -1;

        for(int i = 0; i < localNBaskets; i++){
            if(localBaskets[i] > firstMax){
                secondMax = firstMax;
                secondMaxI = firstMaxI;

                firstMax = localBaskets[i];
                firstMaxI = i;
            } else {
                if(localBaskets[i] > secondMax){
                    secondMax = localBaskets[i];
                    secondMaxI = i;
                }
            }
        }

        PointOfInterest firstPoint(points[index]);
        firstPoint.setAngle(static_cast<double>(firstMaxI * localBasketSize));
        orientPoints << firstPoint;

        if(secondMax >= (firstMax * 0.8)){
            PointOfInterest otherPoint(points[index]);
            otherPoint.setAngle (static_cast<double>(secondMaxI * localBasketSize));
            orientPoints << otherPoint;
        }

    }
    return orientPoints;

}
