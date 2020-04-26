#ifndef CONVOLUTIONALTOOL_H
#include <convolutionaltool.h>
#define CONVOLUTIONALTOOL_H


class SequentialConvolutionalTool : public ConvolutionalTool
{
public:
    SequentialConvolutionalTool() {}
    SequentialConvolutionalTool(int w, int h, double *kernel, int kernelSize, double* yKernel = 0)
        : ConvolutionalTool(w, h, kernel, kernelSize, yKernel) {};
    virtual int* process(FillType ft, Canal type, int pixels[]);
};

#endif // CONVOLUTIONALTOOL_H
