//
// Created by Sergei on 22.05.2020.
//

#ifndef IMAGEPROCESSING_DISTORTION_H
#define IMAGEPROCESSING_DISTORTION_H


#include <QtGui/QPixmap>

class Distortion {
public:
    Distortion() {};
    virtual QPixmap distort(QPixmap pixmap) = 0;
};


#endif //IMAGEPROCESSING_DISTORTION_H
