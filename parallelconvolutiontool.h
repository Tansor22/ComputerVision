#ifndef PARALLELCONVOLUTIONALTOOL_H
#define PARALLELCONVOLUTIONALTOOL_H

#include <QDebug>
#include <thread>
#include <QThread>
#include "convolutiontool.h"
#include "ipworker.h"



class ParallelConvolutionTool : public ConvolutionTool
{
public:
    ParallelConvolutionTool(int w, int h, double *kernel, int kernelSize, double* yKernel = 0)
      : ConvolutionTool(w, h, kernel, kernelSize, yKernel) {}
;
    virtual QRgb* process(FillType ft, Canal type, QRgb*  pixels);
};

#endif // PARALLELCONVOLUTIONALTOOL_H
