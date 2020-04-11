#include "mainform.h"
#include <sandbox.h>
#include <QApplication>

const QString IMAGES_PATH = "C:/Users/Sergei/Documents/QtProjects/images";

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainForm w;
    Sandbox* sb = new Sandbox(&w);
    sb->getImageViaFileDialog();
    int *result = sb->gaussianFilter();
    sb->write(result);
    return a.exec();
}
