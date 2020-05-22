//
// Created by Sergei on 22.05.2020.
//

#ifndef IMAGEPROCESSING_CPPDEMO_H
#define IMAGEPROCESSING_CPPDEMO_H


#include "imagetoprocess.h"

class CppDemo {
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
};


#endif //IMAGEPROCESSING_CPPDEMO_H
