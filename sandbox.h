#ifndef SANDBOX_H
#include "mainform.h"
#include "constants.h"
#include <QFileDialog>
#include <pointofinterest.h>
#include <imagetoprocess.h>
#include <octave.h>
#include <pyramid.h>
#include <descriptor.h>
#include <descriptorbuilder.h>
#include "Distortion.h"
#include "KernelsHandler.h"
#define SANDBOX_H


class Sandbox
{
public:
    MainForm* form;
    QString IMAGES_PATH = "C:/Users/Sergei/Documents/QtProjects/images";
    QString fileName;
    QPixmap imagePixmap;
    double* doubles;
    // meta
    Sandbox(MainForm* form) : form(form){};
    Sandbox();
    void getImageViaFileDialog();
    void getImageViaFileName(const QString& fn);
    void show(QRgb* pixels = nullptr, int w = -1, int h = -1) const;
    void show(ImageToProcess* itp) const;
    void write(QRgb*pixels, const QString& fn = "newImage", int w = -1, int h = -1) const;
    void write(ImageToProcess itp, QString fn = "newImage") const;
    // logic
    QRgb* grayScaled() const;
    double* getDoublesAs(Canal type, int w, int h, double (*mapper)(QRgb) = nullptr) const;
    QRgb* gaussBlurRGB(double sigma) const;
    QRgb* gaussBlurGray(double sigma) const;
    QRgb* increaseSharpness() const;
    QRgb* sobel() const;
    QRgb* sobelV2() const;
    QRgb* descriptors(int nPoints, Distortion* distortion);
    ImageToProcess* moravec(int winSize, int nPoints) const;
    ImageToProcess* harris(int winSize, int nPoints) const;
    void calcPyramid(int nOctaves, int nLevels, double sigmaA, double sigma0);
    void setShowResultsFlagTo(bool value) {showResults = value;}
private:
    bool showResults = true;
    bool innerCall = false;
    QList<Octave*> pyramid;
};

#endif // SANDBOX_H
