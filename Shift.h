//
// Created by Sergei on 23.05.2020.
//

#ifndef IMAGEPROCESSING_SHIFT_H
#define IMAGEPROCESSING_SHIFT_H


#include "Distortion.h"

class Shift : public Distortion {
private:
    int x;
    int y;
public:
    Shift(int _x, int _y) : Distortion(), x(_x), y(_y) {};
    QPixmap distort(QPixmap pixmap) override;
};


#endif //IMAGEPROCESSING_SHIFT_H
