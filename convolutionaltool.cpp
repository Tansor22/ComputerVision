#include "convolutionaltool.h"
#include "constants.h"
#include "helper.cpp"
#include <QDebug>

ConvolutionalTool::ConvolutionalTool()
{

}

ConvolutionalTool::ConvolutionalTool(float *kernel, int kernelSize)
{
    this->kernel = kernel;
    this->kernelSize = kernelSize;
}

int* ConvolutionalTool::setBounds(int fillType, int pixels[], int columns, int gap){
    switch(fillType) {
    case BORDER: {
        int length = sizeof(pixels);
        int rows = length / columns;
        int tmpCols = columns + (2 * gap);
        int tmpRows = rows + (2 * gap);
        int tmpLength = tmpCols * tmpRows;
        int *tmp = new int[tmpLength];

        qDebug() << "Length is " << length << '\n'
                 << "Rows is " << rows << '\n'
                 << "TmpCols is " << tmpCols << '\n'
                 << "TmpRows is " << tmpRows << '\n'
                 << "TmpLength is " << tmpLength << '\n';

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
    }
}

void ConvolutionalTool::process(int w, int h, int pixels[]) {
    int gap = (kernelSize / 2);
    int tmpH = h + (2 * gap);
    int tmpW = w + (2 * gap);
    temp = setBounds(BORDER, pixels, w, gap);
    // doesn't work
    Print(temp);
}
