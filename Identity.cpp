//
// Created by Sergei on 22.05.2020.
//

#include "Identity.h"

Identity::Identity() : Distortion() {

}

QPixmap Identity::distort(QPixmap pixmap) {
    return pixmap;
}
