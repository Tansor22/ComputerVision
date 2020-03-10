#ifndef CONVOLUTIONALTOOL_H
#define CONVOLUTIONALTOOL_H


class ConvolutionalTool
{
public:
    ConvolutionalTool();
    ConvolutionalTool(float *kernel, int kernelSize);
    float *kernel;
    int kernelSize;
    int *temp;
    int* setBounds(int filType, int pixels[], int w, int gap);
    virtual void process(int w, int h, int pixels[]);
};

#endif // CONVOLUTIONALTOOL_H
