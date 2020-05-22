#include "mainform.h"
#include <sandbox.h>
#include <QApplication>
#include "imagehandler.h"

const QString BUTTERFLY = "C:/Users/Sergei/Documents/QtProjects/images/butterfly.jpg";
const QString CAPPED_GIRL = "C:/Users/Sergei/Documents/QtProjects/images/2.jpg";
const QString HOCKEY = "C:/Users/Sergei/Documents/QtProjects/images/hockey.jpg";

void demoHarris(Sandbox* sb);
void demoMoravec(Sandbox* sb);
// * может заходить в функцию со * или без нее.
//В первом случае объект будет имзенен, а во втором - нет.
// не * может заходить в функции без звездочки и с амперсандом при этом конфликт.
// ставим возле параметра амперсанд - конфликт исчерпан,
//но заходит почему то в функцию без амперсанда.
void consume(ImageToProcess& itp);
void consume(ImageToProcess itp);
void consume(ImageToProcess* itp);
void cppDemo();
// с полями класса не работает
void mutate(int& type);
void mutate(double* d);

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainForm w;
    Sandbox* sb = new Sandbox(&w);
    //sb->getImageViaFileName(HOCKEY);
    sb->getImageViaFileDialog();
    sb->descriptors(100);
    delete sb;
    return a.exec();
}

// test
void cppDemo() {
    //ImageToProcess nonPtrObj = ImageToProcess();
    ImageToProcess* ptrObj = new ImageToProcess();
    consume(ptrObj);
    qDebug() << "Out: "<< ptrObj->getType() << endl;
    //int primitive  = 1;
    //mutate(primitive);
     //qDebug() << "Out primitive: "<< primitive << endl;

    mutate(ptrObj->getDoubles());
    // можно писать люой индекс все равно вернет мусор из памяти,
    //НО иногда (если индекс большой валится с ексепшеном)
    // всегда изменяет как массив снаружи
     qDebug() << "Out double pointer: "<< ptrObj->getDoubles()[0] << endl;
}
void consume(ImageToProcess* itp) {
    itp->setType(2);

    qDebug() << "In: "<< itp->getType() << endl;
}
void consume(ImageToProcess itp) {
    itp.setType(2);

    qDebug() << "In: "<< itp.getType() << endl;
}
void consume(ImageToProcess& itp) {
    itp.setType(2);

    qDebug() << "In: "<< itp.getType() << endl;
}
void mutate(int& type) {
    type = -1;
    qDebug() << "In primitive: "<< type << endl;
}
void mutate(double* d) {
    d[0] = -12345;
    qDebug() << "In double pointer: "<< d[0] << endl;
}
void demoHarris(Sandbox* sb) {
    sb->getImageViaFileDialog();
    ImageToProcess toShow = sb->harris(3, 100);
    sb->show(toShow);
}
void demoMoravec(Sandbox* sb) {
    sb->getImageViaFileDialog();
    ImageToProcess toShow = sb->moravek(3, 100);
    sb->show(toShow);
}



