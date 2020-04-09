#include "sequentialconvolutionaltool.h"
#include "constants.h"
#include "helper.h"
#include <dataretriver.h>
#include <QDebug>

SequentialConvolutionalTool::SequentialConvolutionalTool(int w, int h, double *kernel, int kernelSize)
    : ConvolutionalTool(w, h, kernel, kernelSize) {}

int* SequentialConvolutionalTool::process(int pixels[]) {
    int* temp = setBounds(BORDER, pixels);
    //printAs2D(temp, tmpH, tmpW);
    double (*mapper)(int);
    mapper = Helper::normalizeStraight;
    DataRetriver dr = DataRetriver(mapper);
    tempCanals = dr.retriveData(temp, tmpW, tmpH, R | G | B | A);
    //printCanals(tempCanals, tmpH, tmpW);

    canals = new double[w * h * 4];
    int* result = new int[w * h];
    applyKernel(0, w * h, tempCanals, canals, result);
    return result;
}
