//
// Created by Sergei on 23.05.2020.
//

#include "Shift.h"
#include <QDebug>

QPixmap Shift::distort(QPixmap pixmap) {
    QRect r = pixmap.rect();
    r.moveTo(x, y);
    return pixmap.copy(r);
}
