#include "convolutiontool.h"

ConvolutionTool::ConvolutionTool(int w, int h, double *kernel, int kernelSize, double *yKernel) {
    this->h = h;
    this->w = w;
    recharge(kernel, kernelSize, yKernel);
}
void ConvolutionTool::recharge(double *k, int ks, double *yk) {
    kernel = k;
    kernelSize = ks;
    yKernel = yk;
    gap = (ks / 2);
    convGap = (ks / 2);
    tmpH = h + (2 * gap);
    tmpW = w + (2 * gap);
}
// generates temp canvas and sets edges according to fillType param
QRgb* ConvolutionTool::setBounds(FillType ft, QRgb* pixels){
    int tmpCols = w + (2 * gap);
    int tmpRows = h + (2 * gap);
    int tmpLength = tmpCols * tmpRows;
    QRgb* tmp = new QRgb[tmpLength];

//    qDebug() << "Length is " << (w * h) << '\n'
//             << "Rows is " << h << '\n'
//             << "TmpCols is " << tmpCols << '\n'
//             << "TmpRows is " << tmpRows << '\n'
//             << "TmpLength is " << tmpLength << '\n';

    switch(ft) {
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
    case ONES:
    case ZEROS: {
        for (int y = 0; y < tmpLength / tmpCols; y++)
            for (int x = 0; x < tmpCols; x++) {
                tmp[y * tmpCols + x] =
                        (x >= gap && x < w + gap && y >= gap && y < h + gap)
                        ? pixels[(y - gap) * w + (x - gap)]
                        : ft;
            }
        break;
    }

    }
    return tmp;
}

void ConvolutionTool::prepare(FillType ft, Canal type, QRgb* pixels) {
    QRgb *temp = setBounds(ft, pixels);
    //printAs2D(temp, tmpH, tmpW);
    double (*mapper)(QRgb);
    mapper = Helper::normalizeStraight;

    DataRetriever dr = DataRetriever(type, mapper);
    tempCanals = dr.retrieveData(temp, tmpW, tmpH);
    //Helper::printSample(0, 20, tempCanals);
    //printCanals(tempCanals, tmpH, tmpW);

    int canalsCount = Helper::canalsCount(type);

    canals = new double[w * h * canalsCount];
}
// sum
double ConvolutionTool::reduce(double* arr, int length) {
    double sum = 0.0;
    for (int i = 0; i < length; i++) sum += arr[i];
    return sum;
}
// sum by canals, output is double* of by canals reduced values
double* ConvolutionTool::reduce(int canalsCount, double* arr, int size) {
    double* reducedValues = new double[canalsCount];
    for (int c = 0; c < canalsCount; c++) {
        int canalPtr = c * size;
        reducedValues[c] = 0.0;
        for (int i = 0; i < size; i++) reducedValues[c] += arr[i + canalPtr];
    }
    return reducedValues;
}
// clips value, holds it in range min .с. max
int ConvolutionTool::clip(int num, int max, int min) {
    return num < min ? min : qMin(num, max);
}
// clips value, holds it in range min .. max
double ConvolutionTool::clip(double num, double max, double min) {
    return num < min ? min : qMin(num, max);
}
double ConvolutionTool::normalize(double value, double factor, int bias) {
    return factor * value + bias;
}
void ConvolutionTool::applyKernel(
        Canal type,
        int from,
        int to,
        double *tempCanalsParam,
        double *canalsParam,
        QRgb*target,
        double factor,
        int bias
        ) {
    // auxiliary vars
    int k, x, y;
    int it;
    int canalsCount = Helper::canalsCount(type);
    double* values = new double[kernelSize * kernelSize];
    double* yValues;
    // x and y matrix available
    if (yKernel) {
        yValues = new double[kernelSize * kernelSize];
    }
    // each canal, order is R G B A, offset in canalsParam array is w * h for each
    // sequantial processing of all canalsParam!!!
    for (int c = 0; c < canalsCount; c++)
        // rows
        for (it = from; it < to; it++) {
            int offsetY = (it / w) + gap;
            int offsetX = it % w + gap;
            k = 0;
            // convolutional
            for (int i = 0; i < kernelSize; i++)
                for (int j = 0; j < kernelSize; j++) {
                    x = offsetX + j - convGap;
                    y = offsetY + i - convGap;
                    values[k] = tempCanalsParam[y * tmpW + x + (tmpW * tmpH * c)] * kernel[i * kernelSize + j];
                    if (yKernel) yValues[k] = tempCanalsParam[y * tmpW + x + (tmpW * tmpH * c)] * yKernel[i * kernelSize + j];
                    k++;
                }
            double toSet;
            if (yKernel) {
                // reducing x and y
                double xReduced = normalize(reduce(values, kernelSize * kernelSize), factor, bias);
                double yReduced = normalize(reduce(yValues, kernelSize * kernelSize), factor, bias);
                double calculated = sqrt(xReduced * xReduced) + (yReduced * yReduced);
                toSet = sobel
                        ? calculated // no need to cut
                        : clip(calculated, 1.0, 0.0);
            } else {
                // reducing single
                double reduced = reduce(values, kernelSize * kernelSize);
                toSet = sobel
                        ? normalize(reduced, factor, bias) // no need to cut
                        : clip(normalize(reduced, factor, bias), 1.0, 0.0);
            }
            canalsParam[offsetX - convGap + (offsetY - convGap) * (tmpW - 2 * convGap) + h * w * c]
                    = toSet;
        }
    //printCanals(canalsParam, h, w);
    // constructing result

    if (sobel) {
        DataRetriever dr = DataRetriever(GRAY);
        dr.normalizeExtra(to - from, canalsParam);
    }
    for (it = from; it < to; it++)
        target[it] =
                Helper::isGray(type)
                ? qRgb(Helper::normalizeReverse(canalsParam[it]), Helper::normalizeReverse(canalsParam[it]), Helper::normalizeReverse(canalsParam[it]))
                : qRgba(
                      Helper::normalizeReverse(canalsParam[it]), // RED
                      Helper::normalizeReverse(canalsParam[it + w * h]), // GREEN
                Helper::normalizeReverse(canalsParam[it + w * h * 2]), // BLUE
                Helper::noAlpha(type) ? 255 : Helper::normalizeReverse(canalsParam[it + w * h * 3])); // ALPHA
};
