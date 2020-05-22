//
// Created by Sergei on 22.05.2020.
//

#include "CppDemo.h"

// test
void CppDemo::cppDemo() {
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
void CppDemo::consume(ImageToProcess* itp) {
    itp->setType(2);

    qDebug() << "In: "<< itp->getType() << endl;
}
void CppDemo::consume(ImageToProcess itp) {
    itp.setType(2);

    qDebug() << "In: "<< itp.getType() << endl;
}
void CppDemo::consume(ImageToProcess& itp) {
    itp.setType(2);

    qDebug() << "In: "<< itp.getType() << endl;
}
void CppDemo::mutate(int& type) {
    type = -1;
    qDebug() << "In primitive: "<< type << endl;
}
void CppDemo::mutate(double* d) {
    d[0] = -12345;
    qDebug() << "In double pointer: "<< d[0] << endl;
}