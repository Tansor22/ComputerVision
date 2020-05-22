//
// Created by Sergei on 22.05.2020.
//

#ifndef IMAGEPROCESSING_ROTATE_H
#define IMAGEPROCESSING_ROTATE_H


#include "Distortion.h"

class Rotate : public Distortion {
private:
    double degrees;
public:
    explicit Rotate(double degrees) : degrees(degrees) {};
    virtual QPixmap distort(QPixmap pixmap);
};


#endif //IMAGEPROCESSING_ROTATE_H
