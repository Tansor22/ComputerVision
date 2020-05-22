#include "mainform.h"
#include <sandbox.h>
#include <QApplication>
#include "imagehandler.h"
#include "Identity.h"
#include "Rotate.h"

const QString BUTTERFLY = "C:/Users/Sergei/Documents/QtProjects/images/butterfly.jpg";
const QString CAPPED_GIRL = "C:/Users/Sergei/Documents/QtProjects/images/2.jpg";
const QString HOCKEY = "C:/Users/Sergei/Documents/QtProjects/images/hockey.jpg";

// lab 1
void demoSobel(Sandbox *sb);

void demoGauss(Sandbox *sb);

// lab 2
void demoPyramids(Sandbox *sb);

//lab3
void demoHarris(Sandbox *sb);

void demoMoravec(Sandbox *sb);

// lab 4 and further
void demoDescriptors(Sandbox *sb, Distortion *distortion);

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    MainForm w;
    Sandbox *sb = new Sandbox(&w);
    // Custom
    /*
    sb->getImageViaFileName(HOCKEY);
    ImageToProcess i = new ImageToProcess(sb->imagePixmap, GRAY);
    i.gradient();
    sb->show(i);
     */

    //demoSobel(sb);

    //demoGauss(sb);

    demoMoravec(sb);

    //demoHarris(sb);

    //demoPyramids(sb);

    //demoDescriptors(sb, new Identity());

    //demoDescriptors(sb, new Rotate(15));
    delete sb;
    return QApplication::exec();
}

void demoSobel(Sandbox *sb) {
    sb->getImageViaFileDialog();
    sb->sobel();
}

void demoGauss(Sandbox *sb) {
    sb->getImageViaFileDialog();
    sb->gaussBlurRGB(3.1);
}

void demoPyramids(Sandbox *sb) {
    sb->getImageViaFileName(HOCKEY);
    sb->calcPyramid(2, 3, 0.0, 1.5);
}

void demoDescriptors(Sandbox *sb, Distortion *distortion) {
    sb->getImageViaFileName(HOCKEY);
    sb->descriptors(100, distortion);
}

void demoHarris(Sandbox *sb) {
    sb->getImageViaFileDialog();
    ImageToProcess toShow = sb->harris(3, 100);
    sb->show(toShow);
}

void demoMoravec(Sandbox *sb) {
    sb->getImageViaFileDialog();
    ImageToProcess toShow = sb->moravec(3, 100);
    sb->show(toShow);
}



