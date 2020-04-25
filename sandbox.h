#ifndef SANDBOX_H
#include "mainform.h"
#include "constants.h"
#include <QFileDialog>
#include <pointofinterest.h>
#define SANDBOX_H


class Sandbox
{
private:
    MainForm* form;
    QString IMAGES_PATH = "C:/Users/Sergei/Documents/QtProjects/images";
    QString fileName;
    QPixmap imagePixmap;

public:
    // meta
    Sandbox(MainForm* form) : form(form){};
    void getImageViaFileDialog();
    void getImageViaFileName(QString fileName);
    void show(int* pixels = 0);
    void write(int *pixels);
    // logic
    int* grayscaled();
    int* gaussBlurRGB(double sigma);
    int* gaussBlurGray(double sigma);
    int* gaussBlurGrayV2(double sigma);
    int* increaseSharpness();
    int* sobel();
    int* sobelV2();
    int* moravek(int winSize, int nPoints);
    void setShowResultsFlagTo(bool value) {showResults = value;}
    ConvolutionalTool* tool;
private:
    bool showResults = true;
};

#endif // SANDBOX_H
