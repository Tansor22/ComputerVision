#include "convolutionaltool.h"

ConvolutionalTool::ConvolutionalTool(int w, int h, double *kernel, int kernelSize) {
    this->kernel = kernel; this->kernelSize = kernelSize;
    gap = (kernelSize / 2);
    convGap = (kernelSize / 2);
    this->h = h; this->w = w;

    tmpH = h + (2 * gap);
    tmpW = w + (2 * gap);
}
// generates temp canvas and sets edges according to fillType param
int* ConvolutionalTool::setBounds(int fillType, int pixels[]){
    int tmpCols = w + (2 * gap);
    int tmpRows = h + (2 * gap);
    int tmpLength = tmpCols * tmpRows;
    int *tmp = new int[tmpLength];

    qDebug() << "Length is " << (w * h) << '\n'
             << "Rows is " << h << '\n'
             << "TmpCols is " << tmpCols << '\n'
             << "TmpRows is " << tmpRows << '\n'
             << "TmpLength is " << tmpLength << '\n';

    switch(fillType) {
    // blackbox
    case BORDER: {
        for (int y = 0; y < tmpLength / tmpCols; y++)
            for (int x = 0; x < tmpCols; x++) {
                // by changing these vars' names it's possible to populate differently
                // e.g. isBottom => isTop, isTop => isBottom ==> populating will be changed
                bool isRight = x >= gap;
                bool isLeft = x < w + gap;
                bool isBottom = y >= gap;
                bool isTop = y < h + gap;
                int toSet;
                if (isLeft && isRight && isTop && isBottom)
                    toSet = pixels[(y - gap) * w + (x - gap)];
                else if (isRight && isTop && isLeft)
                    toSet = pixels[x - gap];
                else if (isLeft && isTop && isBottom)
                    // right
                    toSet =  pixels[w * (y - gap)];
                else if (isRight && isTop && isBottom)
                    // left
                    toSet =  pixels[(y - gap) * w + (w - 1)];
                else if (isRight && isLeft && isBottom)
                    // bottom
                    toSet =  pixels[(h - 1) * w + (x - gap)];

                // angles
                else if (isTop && isLeft)
                    // left upper angle
                    toSet =  pixels[0];
                else if (isLeft && isBottom)
                    // left bottom angle
                    toSet =  pixels[(h - 1) * w];
                else if (isRight && isTop)
                    // right upper angle
                    toSet =  pixels[w - 1];
                //isRight && isBottom
                // right bottom angle
                else toSet =  pixels[h * w - 1];
                // setting
                tmp[y * tmpCols + x] = toSet;
            }
        break;

    }
    case ZEROS: {
        for (int y = 0; y < tmpLength / tmpCols; y++)
            for (int x = 0; x < tmpCols; x++) {
                tmp[y * tmpCols + x] =
                        (x >= gap && x < w + gap && y >= gap && y < h + gap)
                        ? pixels[(y - gap) * w + (x - gap)]
                        : 0;
            }
        break;
    }
    }
    return tmp;
}

// sum
double ConvolutionalTool::reduce(double* arr, int length) {
    double sum = 0.0;
    for (int i = 0; i < length; i++) sum += arr[i];
    return sum;
}
// clips value, holds it in range min .. max
int ConvolutionalTool::clip(int num, int max, int min) {
    return num < min ? min : qMin(num, max);
}
// clips value, holds it in range min .. max
double ConvolutionalTool::clip(double num, double max, double min) {
    return num < min ? min : qMin(num, max);
}
void ConvolutionalTool::applyKernel(
        int from,
        int to,
        double *tempCanals,
        double *canals,
        int *target
       ) {
    // auxiliary vars
    int k, x, y;
    int it;
    double* values = new double[kernelSize * kernelSize];
    // each canal, order is R G B A, offset in canals array is w * h for each
    for (int c = 0; c < 4; c++)
        // rows
        for (it = from; it < to; it++) {
            int tmpY = (it / w) + gap;
            int tmpX = it % w + gap;
            k = 0;
            // convolutional
            for (int i = 0; i < kernelSize; i++)
                for (int j = 0; j < kernelSize; j++) {
                    x = tmpX + j - convGap;
                    y = tmpY + i - convGap;
                    values[k++] = tempCanals[y * tmpW + x + (tmpW * tmpH * c)] * kernel[i * kernelSize + j];
                }

            // reducing
            canals[tmpX - convGap + (tmpY - convGap) * (tmpW - 2 * convGap) + h * w * c]
                    = clip(reduce(values, kernelSize * kernelSize), 1.0, 0.0);
        }
    //printCanals(canals, h, w);
    // constructing result
    for (it = from; it < to; it++)
        target[it] =
                qRgba(
                    Helper::normalizeReverse(canals[it]), // RED
                    Helper::normalizeReverse(canals[it + w * h]), // GREEN
                Helper::normalizeReverse(canals[it + w * h * 2]), // BLUE
                Helper::normalizeReverse(canals[it + w * h * 3])); // ALPHA
};
