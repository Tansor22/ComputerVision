#ifndef SANDBOX_H
#include "mainform.h"
#include "constants.h"
#include <QFileDialog>
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
    int* gaussianFilter();
    int* increaseSharpness();
    int* sobelOperator();
};

#endif // SANDBOX_H
