#ifndef DESCRIPTORBUILDER_H
#include <imagetoprocess.h>
#define DESCRIPTORBUILDER_H
#include <pointofinterest.h>
#include <descriptor.h>

class DescriptorBuilder
{
public:
    int w, h;
    int nBaskets;
    int nHistograms;
    int gridSize;

    double *gaussKernel;

    ImageToProcess *dx;
    ImageToProcess *dy;
    ImageToProcess *gradient;
    ImageToProcess *gradientDirection;
    DescriptorBuilder(ImageToProcess *itp, int nBaskets = 8, int nHistogram = 4, int gridSize = 16);

    Descriptor createDescriptor(PointOfInterest inputPoint);
    QList<PointOfInterest> orientPoints(QList<PointOfInterest> points);
};

#endif // DESCRIPTORBUILDER_H
