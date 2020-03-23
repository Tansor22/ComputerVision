#ifndef CONVOLUTIONALTOOL_H
#define CONVOLUTIONALTOOL_H


class ConvolutionalTool
{
public:
    ConvolutionalTool();
    ConvolutionalTool(float *kernel, int kernelSize);
    float *kernel;
    int kernelSize;
    int* temp;
    int* tempCanals;
    int* canals;
    int clip(int num, int max, int min);
    int reduce(int* arr, int length);
    int* setBounds(int filType, int pixels[], int columns, int rows, int gap);
    virtual int* process(int w, int h, int pixels[]);
};

#endif // CONVOLUTIONALTOOL_H
