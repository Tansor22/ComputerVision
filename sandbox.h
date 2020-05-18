#ifndef SANDBOX_H
#include "mainform.h"
#include "constants.h"
#include <QFileDialog>
#include <pointofinterest.h>
#include <imagetoprocess.h>
#include <octave.h>
#include <pyramid.h>
#include<descriptor.h>
#include<descriptorbuilder.h>
#define SANDBOX_H


class Sandbox
{
private:


public:
    MainForm* form;
    QString IMAGES_PATH = "C:/Users/Sergei/Documents/QtProjects/images";
    QString fileName;
    QPixmap imagePixmap;
    double* doubles;
    QList<PointOfInterest> pois;
    // meta
    Sandbox(MainForm* form) : form(form){};
    Sandbox();
    void getImageViaFileDialog();
    void getImageViaFileName(QString fileName);
    void show(int* pixels = 0, int w = -1, int h = -1);
    void show(ImageToProcess itp);
    void write(int *pixels, QString fileName = "newImage", int w = -1, int h = -1);
    void write(ImageToProcess itp, QString fileName = "newImage");
    // logic
    int* grayscaled();
    double* getDoublesAs(Canal type, int w, int h, double (*mapper)(int) = NULL);
    int* gaussBlurRGB(double sigma);
    int* gaussBlurGray(double sigma);
    int* gaussBlurGrayV2(double sigma);
    int* crossDemo();
    int* increaseSharpness();
    int* sobel();
    int* sobelV2();
    int* descriptors(int nPoints);
    ImageToProcess moravek(int winSize, int nPoints);
    ImageToProcess harris(int winSize, int nPoints);
    void calcPyramid(int nOctaves, int nLevels, double sigmaA, double sigma0);
    void setShowResultsFlagTo(bool value) {showResults = value;}
    ConvolutionalTool* tool;
private:
    bool showResults = true;
    bool innerCall = false;
    QList<Octave*> pyramid;
};

#endif // SANDBOX_H
