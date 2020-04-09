#ifndef CONVOLUTIONALTOOL_H
#define CONVOLUTIONALTOOL_H

#include "helper.h"
#include "dataretriver.h"



class ConvolutionalTool
{
public:
    ConvolutionalTool(int w, int h, double *kernel, int kernelSize);
    virtual ~ConvolutionalTool() { }
    virtual int* process(int pixels[]) = 0;
    double reduce(double* arr, int length);
    int clip(int num, int max, int min);
    double clip(double num, double max, double min);
    void applyKernel(bool gray, int from, int to, double *tempCanals, double *canals, int *target);
protected:
    int kernelSize;
    double *kernel;
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
