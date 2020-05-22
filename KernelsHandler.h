//
// Created by Sergei on 22.05.2020.
//

#ifndef IMAGEPROCESSING_KERNELSHANDLER_H
#define IMAGEPROCESSING_KERNELSHANDLER_H


class KernelsHandler {
public:
    static double* getSobelX();
    static double* getSobelY();
    static double* getIncreaseSharpness();
    static double* getGauss(double sigma);

};


#endif //IMAGEPROCESSING_KERNELSHANDLER_H
