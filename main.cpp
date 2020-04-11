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
    //sb->getImageViaFileName(BUTTERFLY);
    int *result = sb->sobelOperator();
    sb->write(result);
    return a.exec();
}
