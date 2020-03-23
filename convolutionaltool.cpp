#include "convolutionaltool.h"
#include "constants.h"
#include "helper.cpp"
#include <dataretriver.h>
#include <QDebug>

ConvolutionalTool::ConvolutionalTool()
{

}

ConvolutionalTool::ConvolutionalTool(float *kernel, int kernelSize)
{
    this->kernel = kernel;
    this->kernelSize = kernelSize;
}

// generates temp canvas and sets edges according to fillType param
int* ConvolutionalTool::setBounds(int fillType, int pixels[], int columns, int rows, int gap){
    int tmpCols = columns + (2 * gap);
    int tmpRows = rows + (2 * gap);
    int tmpLength = tmpCols * tmpRows;
    int *tmp = new int[tmpLength];

    qDebug() << "Length is " << (rows * columns) << '\n'
             << "Rows is " << rows << '\n'
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
                bool isLeft = x < columns + gap;
                bool isBottom = y >= gap;
                bool isTop = y < rows + gap;
                int toSet;
                if (isLeft && isRight && isTop && isBottom)
                    toSet = pixels[(y - gap) * columns + (x - gap)];
                else if (isRight && isTop && isLeft)
                    toSet = pixels[x - gap];
                else if (isLeft && isTop && isBottom)
                    // right
                    toSet =  pixels[columns * (y - gap)];
                else if (isRight && isTop && isBottom)
                    // left
                    toSet =  pixels[(y - gap) * columns + (columns - 1)];
                else if (isRight && isLeft && isBottom)
                    // bottom
                    toSet =  pixels[(rows - 1) * columns + (x - gap)];

                // angles
                else if (isTop && isLeft)
                    // left upper angle
                    toSet =  pixels[0];
                else if (isLeft && isBottom)
                    // left bottom angle
                    toSet =  pixels[(rows - 1) * columns];
                else if (isRight && isTop)
                    // right upper angle
                    toSet =  pixels[columns - 1];
                //isRight && isBottom
                // right bottom angle
                else toSet =  pixels[rows * columns - 1];
                // setting
                tmp[y * tmpCols + x] = toSet;
            }
        break;

    }
    case ZEROS: {
        for (int y = 0; y < tmpLength / tmpCols; y++)
            for (int x = 0; x < tmpCols; x++) {
                tmp[y * tmpCols + x] =
                        (x >= gap && x < columns + gap && y >= gap && y < rows + gap)
                        ? pixels[(y - gap) * columns + (x - gap)]
                        : 0;
            }
        break;
    }
    }
    return tmp;
}
// clips value, holds it in range min .. max
int ConvolutionalTool::clip(int num, int max, int min) {
    return num < min ? min : qMin(num, max);
}

// sum
int ConvolutionalTool::reduce(int* arr, int length) {
    int sum = 0;
    for (int i = 0; i < length; i++) sum += arr[i];
    return sum;
}

// main logic
int* ConvolutionalTool::process(int w, int h, int pixels[]) {
    int gap = (kernelSize / 2);
    int tmpH = h + (2 * gap);
    int tmpW = w + (2 * gap);
    temp = setBounds(BORDER, pixels, w, h, gap);
    //printAs2D(temp, tmpH, tmpW);
    DataRetriver dr = DataRetriver();
    tempCanals = dr.retriveData(temp, tmpW, tmpH, R | G | B);
    //printCanals(tempCanals, tmpH, tmpW);

    canals = new int[w * h * 3];
    int* values = new int[kernelSize * kernelSize];
    int convGap = (kernelSize / 2);
    int k, x, y;
    // each canal, order is R G B, offset in canals array is w * h for each
    for (int c = 0; c < 3; c++)
        // rows
        for (int tmpY = gap; tmpY < h + gap; tmpY++)
            // columns
            for (int tmpX = gap; tmpX < w + gap; tmpX++) {
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
                        = clip(reduce(values, kernelSize * kernelSize), 255, 0);
            }
    //printCanals(canals, h, w);
    int* result = new int[w * h];
    // constructing result
    for (y = 0; y < h; y++)
        for (x = 0; x < w; x++)
            result[y * w + x] =
                    qRgb(canals[y * w + x], // RED
                    canals[y * w + x + w * h], // GREEN
                    canals[y * w + x + w * h *2]); // BLUE
    //printAs2D(result, h, w);
    return result;
}
