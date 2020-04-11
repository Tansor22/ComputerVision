#ifndef CONVOLUTIONALTOOL_H
#define CONVOLUTIONALTOOL_H

#include "helper.h"
#include "dataretriver.h"



class ConvolutionalTool
{
public:
    ConvolutionalTool(int w, int h, double *kernel, int kernelSize, double *yKernel = 0);
    virtual ~ConvolutionalTool() { delete tempCanals; delete canals;}
    virtual int* process(FillType ft, Canal type, int* pixels) = 0;
    void prepare(FillType ft, Canal type, int* pixels);
    double reduce(double* arr, int length);
    double normalize(double value, double factor, int bias);
    int clip(int num, int max, int min);
    double clip(double num, double max, double min);
    void applyKernel(Canal type, int from, int to, double *tempCanals, double *canals, int *target,
                     double factor = 1.0,
                     int bias = 0);
protected:
    int kernelSize;
    double *kernel;
    double *yKernel;
    int gap;
    int convGap;
    int h;
    int w;
    int tmpH;
    int tmpW;
    double *tempCanals;
    double *canals;
    int* setBounds(FillType ft, int pixels[]);

};

#endif // CONVOLUTIONALTOOL_H
