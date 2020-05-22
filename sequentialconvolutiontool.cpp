#include "sequentialconvolutiontool.h"
#include <dataretriver.h>
#include <QDebug>

QRgb* SequentialConvolutionTool::process(FillType ft, Canal type, QRgb*  pixels) {
    // setting borders, data retrieving, preparing canals
    prepare(ft, type, pixels);
    // result pixels
    QRgb* result = new QRgb[w * h];
    applyKernel(type, 0, w * h, tempCanals, canals, result);
    return result;
}
