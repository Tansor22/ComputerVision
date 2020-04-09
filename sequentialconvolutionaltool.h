#ifndef CONVOLUTIONALTOOL_H
#include <convolutionaltool.h>
#define CONVOLUTIONALTOOL_H


class SequentialConvolutionalTool : public ConvolutionalTool
{
public:
    SequentialConvolutionalTool(int w, int h, double *kernel, int kernelSize);
    virtual int* process(int pixels[]);
};

#endif // CONVOLUTIONALTOOL_H