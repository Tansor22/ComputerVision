#ifndef PARALLELCONVOLUTIONALTOOL_H
#define PARALLELCONVOLUTIONALTOOL_H

#include <QDebug>
#include <thread>
#include <QThread>
#include "convolutionaltool.h"
#include "ipworker.h"



class ParallelConvolutionalTool : public ConvolutionalTool
{
public:
    ParallelConvolutionalTool(int w, int h, double *kernel, int kernelSize);
    virtual int *process(FillType ft, Canal type, int* pixels);
};

#endif // PARALLELCONVOLUTIONALTOOL_H
