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
    MainForm* form;
    QString IMAGES_PATH = "C:/Users/Sergei/Documents/QtProjects/images";
    QString fileName;
    QPixmap imagePixmap;
    double* doubles;
    QList<PointOfInterest> pois;

public:
    // meta
    Sandbox(MainForm* form) : form(form){};
    Sandbox();
    void getImageViaFileDialog();
    void getImageViaFileName(QString fileName);
    void show(int* pixels = 0);
    void write(int *pixels, QString fileName = "newImage");
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
    int* moravek(int winSize, int nPoints);
    int* harris(int winSize, int nPoints);
    void calcPyramid(int nOctaves, int nLevels, double sigmaA, double sigma0);
    void setShowResultsFlagTo(bool value) {showResults = value;}
    ConvolutionalTool* tool;
private:
    bool showResults = true;
    bool innerCall = false;
    QList<Octave*> pyramid;
};

#endif // SANDBOX_H
