#ifndef CONVOLUTIONALTOOL_H
#include <convolutiontool.h>
#define CONVOLUTIONALTOOL_H


class SequentialConvolutionTool : public ConvolutionTool
{
public:
    SequentialConvolutionTool() = default;
    SequentialConvolutionTool(int w, int h, double *kernel, int kernelSize, double* yKernel = 0)
        : ConvolutionTool(w, h, kernel, kernelSize, yKernel) {};
    QRgb* process(FillType ft, Canal type, QRgb* pixels) override;
};

#endif // CONVOLUTIONALTOOL_H
