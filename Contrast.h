//
// Created by Sergei on 23.05.2020.
//

#ifndef IMAGEPROCESSING_CONTRAST_H
#define IMAGEPROCESSING_CONTRAST_H


#include "Distortion.h"
#include "dataretriver.h"

class Contrast : public Distortion {
private:
    int correction;
public:
    Contrast(int _correction);

    QPixmap distort(QPixmap pixmap) override;
};


#endif //IMAGEPROCESSING_CONTRAST_H
