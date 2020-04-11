#include "sequentialconvolutionaltool.h"
#include "constants.h"
#include "helper.h"
#include <dataretriver.h>
#include <QDebug>

int* SequentialConvolutionalTool::process(FillType ft, Canal type, int* pixels) {
    // setting borders, data retreiving, preparing canals
    prepare(ft, type, pixels);
    // result pixels
    int* result = new int[w * h];
    applyKernel(type, 0, w * h, tempCanals, canals, result);
    return result;
}
