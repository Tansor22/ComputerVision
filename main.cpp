#include "mainform.h"
#include <sandbox.h>
#include <QApplication>

const QString BUTTERFLY = "C:/Users/Sergei/Documents/QtProjects/images/butterfly.jpg";
const QString CAPPED_GIRL = "C:/Users/Sergei/Documents/QtProjects/images/2.jpg";

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainForm w;
    Sandbox* sb = new Sandbox(&w);
    sb->getImageViaFileDialog();
    ImageToProcess itp = ImageToProcess(sb->imagePixmap, GRAY);
    //sb->calcPyramid(2, 3, 0, 1);
    //itp.derivativeY();
    itp.gaussBlur(1.6);


    //sb->getImageViaFileName(BUTTERFLY);
   int* result = itp.toIntRGB();
    //int *result = sb->moravek(5, 400);//sb->harris(5, 800);//sb->moravek(5, 10);
    //sb->calcPyramid(3, 2, 0, 1);
    //sb->gaussBlurGrayV2(1.3);
    //sb->descriptors(10);
    //sb->gaussBlurRGB(4.5);
    //sb->calcPyramid(3, 2, 1.6, 2.0);
    sb->write(result);
    sb->show(result);
    return a.exec();
}
