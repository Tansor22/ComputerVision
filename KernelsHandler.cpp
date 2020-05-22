//
// Created by Sergei on 22.05.2020.
//

#include "KernelsHandler.h"
#include "helper.h"

double *KernelsHandler::getSobelX() {
    return new double[9] {
            -1.0, 0, 1.0,
            -2.0, 0, 2.0,
            -1.0, 0, 1.0
    };
}

double *KernelsHandler::getSobelY() {
    return new double[9] {
            -1.0, -2.0, -1.0,
            0.0, 0.0, 0.0,
            1.0, 2.0, 1.0
    };
}

double *KernelsHandler::getIncreaseSharpness() {
    return new double[9] {
            -1.0, -1.0, -1.0,
            -1.0, 9.0, -1.0,
            -1.0, -1.0, -1.0
    };
}

double *KernelsHandler::getGauss(double sigma) {
    return Helper::gauss(sigma);
}
