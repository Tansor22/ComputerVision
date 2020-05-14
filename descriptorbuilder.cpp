#include "descriptorbuilder.h"

DescriptorBuilder::DescriptorBuilder(ImageToProcess *itp, int nBaskets, int nHistogram, int gridSize)
{
    int w = itp->getW();
    int h = itp->getH();

    // searching for gradient
    gradient = new ImageToProcess(GRAY, itp->getDoubles(), w, h);
    gradient->gradient();

    // searching for derivatives
    dx = new ImageToProcess(GRAY, itp->getDoubles(), w, h);
    dy = new ImageToProcess(GRAY, itp->getDoubles(), w, h);
    dx->derivativeX();
    dy->derivativeY();

    //находим направление градиента
    gradientDirection = new ImageToProcess(GRAY, itp->getDoubles(), w, h);
    for (int j = 0; j < h; j++) {
        for (int i = 0; i < w; i++) {
            double gradDir = atan2(dy->getValueSafe(i, j),
                                   dx->getValueSafe(i, j)) * 180.0 / M_PI + 180.0;  // magic
            gradientDirection->setValueSafe(i, j, gradDir);
        }
    }

    //Sandbox::write(*gradientDirection, "gradientDirection");

    // TODO to check whether it's correct
    gaussKernel = Helper::gauss(gridSize / 6);


}
