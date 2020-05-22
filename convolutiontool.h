#ifndef CONVOLUTIONTOOL_H
#define CONVOLUTIONTOOL_H
#include "helper.h"
#include "dataretriver.h"





class ConvolutionTool
{
public:
    ConvolutionTool() {};
    ConvolutionTool(int w, int h, double *kernel, int kernelSize, double *yKernel = 0);
    virtual ~ConvolutionTool() { delete tempCanals; delete canals;}
    void recharge(double *k, int ks, double *yk = 0);
    virtual QRgb* process(FillType ft, Canal type, QRgb* pixels) = 0;
    void prepare(FillType ft, Canal type, QRgb* pixels);
    double reduce(double* arr, int length);
    static double* reduce(int canalsCount, double* arr, int length);
    double normalize(double value, double factor, int bias);
    void setSobelFlagTo(bool value) {sobel = value;}
    double* getCanals() { return canals;}
    int clip(int num, int max, int min);
    static double clip(double num, double max, double min);
    void applyKernel(Canal type, int from, int to, double *tempCanalsParam, double *canalsParam, QRgb*target,
                     double factor = 1.0,
                     int bias = 0);
protected:
    QRgb* setBounds(FillType ft, QRgb* pixels);
    int kernelSize;
    double *kernel;
    double *yKernel;
    int gap;
    int convGap;
    int h;
    int w;
    int tmpH;
    int tmpW;
    bool sobel = false;
    double *tempCanals;
    double *canals;

};

#endif // CONVOLUTIONTOOL_H
