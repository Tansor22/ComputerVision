//
// Created by Sergei on 22.05.2020.
//

#ifndef IMAGEPROCESSING_IDENTITY_H
#define IMAGEPROCESSING_IDENTITY_H

#include "Distortion.h"

class Identity : public Distortion {
public:
    Identity();
    virtual QPixmap distort(QPixmap pixmap);
};


#endif //IMAGEPROCESSING_IDENTITY_H
