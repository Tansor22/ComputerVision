#include "imagetoprocess.h"
#include "sandbox.h"

void Sandbox::show(int *pixels) {
    if (showResults) {
        if (pixels == 0) {
            // show current src image
            form->scene->addPixmap(imagePixmap);
        } else {
            QImage* image = new QImage(imagePixmap.width(), imagePixmap.height(), QImage::Format_ARGB32);

            for (int y = 0; y < imagePixmap.height(); y++)
                for (int x = 0; x < imagePixmap.width(); x++)
                    image->setPixelColor(x,y, pixels[y * imagePixmap.width() + x]);
            form->scene->addPixmap(QPixmap::fromImage(*image));
        }
        form->showMaximized();
    }
}
void Sandbox::getImageViaFileName(QString fileName)  {
    this->fileName = fileName;
    imagePixmap.load(fileName);
}
void Sandbox::getImageViaFileDialog() {

    fileName = QFileDialog::
            getOpenFileName(NULL,
                            "Open Image", IMAGES_PATH, "Image Files (*.png *.jpg *.bmp)");
    imagePixmap.load(fileName);
}
void Sandbox::write(int *pixels) {
    QImage image = imagePixmap.toImage();

    for (int y = 0; y < imagePixmap.height(); y++)
        for (int x = 0; x < imagePixmap.width(); x++)
            image.setPixelColor(x,y, pixels[y * imagePixmap.width() + x]);

    QFile file("newImage.png");
    file.open(QIODevice::WriteOnly);
    image.save(&file, "PNG");
}

int* Sandbox::grayscaled() {
    DataRetriver dr = DataRetriver(NULL);

    int* data = dr.retriveData(imagePixmap);
    // such a retriver returns single channel(gray) 0..255 (no mapper) as double
    dr = DataRetriver(GRAY);
    double* grayscaled = dr.retriveData(data,imagePixmap.width(), imagePixmap.height());
    int size = imagePixmap.width() * imagePixmap.height();
    data = new int[size];
    for (int i = 0; i < size; i ++)
        data[i] = qRgb(grayscaled[i], grayscaled[i], grayscaled[i]);

    show(data);
    return data;
}
int* Sandbox::gaussBlurGrayV2(double sigma) {
    QList<QList<double>> kernel;

    double s = sigma * sigma * 2;

    int halfSize = static_cast<int>(sigma) * 3;
    int size = halfSize * 2;
    if (halfSize  % 2 == 0)
        ++halfSize;

    QList<double> str;
    for (int i = -halfSize; i <= halfSize; i++){
        str.append(exp(- i * i / s) / (M_PI * s));
    }
    kernel.append(str);


    tool = new ParallelConvolutionalTool(imagePixmap.width(),
                                         imagePixmap.height(),
                                         gauss,
                                         halfSize * 2);

    DataRetriver dr = DataRetriver(NULL);

    int* data = dr.retriveData(imagePixmap); //непосредственно вычисляем


    kernel.clear();
    for (int i = -halfSize; i <= halfSize; i++){
        QList<double> str;
        str.append(exp(- i * i / s) / (M_PI * s));
        kernel.append(str);
    }

    imageMono->convolutionUniversal(core, true); //непосредственно вычисляем
}
int* Sandbox::gaussBlurGray(double sigma) {
    int size = floor(3 * sigma);
    double* kernel =  Helper::gauss(sigma);

    tool = new ParallelConvolutionalTool(imagePixmap.width(),
                                         imagePixmap.height(),
                                         kernel,
                                         size);

    DataRetriver dr = DataRetriver(NULL);

    int* data = dr.retriveData(imagePixmap);
    int * result = tool->process(BORDER, GRAY, data);
    show(result);
    return result;
}
int* Sandbox::gaussBlurRGB(double sigma) {
    int size = floor(3 * sigma);
    double* kernel =  Helper::gauss(sigma);

    tool = new ParallelConvolutionalTool(imagePixmap.width(),
                                         imagePixmap.height(),
                                         kernel,
                                         size);

    DataRetriver dr = DataRetriver(NULL);

    int* data = dr.retriveData(imagePixmap);
    int * result = tool->process(BORDER, R | G | B | A, data);
    show(result);
    return result;
}
int* Sandbox::increaseSharpness() {
    double INCREASE_SHARPNESS[] =  {
        -1.0, -1.0, -1.0,
        -1.0, 9.0, -1.0,
        -1.0, -1.0, -1.0
    };

    int size = 3;


    ConvolutionalTool* tool = new ParallelConvolutionalTool(imagePixmap.width(),
                                                            imagePixmap.height(),
                                                            INCREASE_SHARPNESS,
                                                            size);

    DataRetriver dr = DataRetriver(NULL);

    int* data = dr.retriveData(imagePixmap);
    int * result = tool->process(BORDER, R | G | B | A, data);
    show(result);
    return result;
}
int* Sandbox::sobelV2() {
    double SOBEL_X[] =  {
        1.0, 0, -1.0,
        2.0, 0, -2.0,
        1.0, 0, -1.0
    };
    int size = 3;
    int w = imagePixmap.width();
    int h = imagePixmap.height();
    ConvolutionalTool* tool = new SequentialConvolutionalTool(w, h, SOBEL_X, size);
    tool->setSobelFlagTo(true);
    DataRetriver dr = DataRetriver(NULL);

    int* data = dr.retriveData(imagePixmap);
    // ignoring the result
    tool->process(BORDER, GRAY, data);
    double* derivativeX = Helper::copyOf(tool->getCanals(), w * h);

    // y derivative
    double SOBEL_Y[] =  {
        1.0, 2.0, 1.0,
        0.0, 0.0, 0.0,
        -1.0, -2.0, -1.0
    };
    tool->recharge(SOBEL_Y, size);
    // ignoring the result
    tool->process(BORDER, GRAY, data);
    // no need to copy
    double* derivativeY = tool->getCanals();

    double* temp = new double[w * h];
    int* result = new int[w * h];

    for (int i = 0; i < w * h; i++)
        temp[i] = sqrt(derivativeY[i] * derivativeY[i] +
                       derivativeX[i] * derivativeX[i]);

    dr = DataRetriver(GRAY);
    dr.normalizeExtra(w *h, temp);

    result = Helper::toIntRGB(GRAY, temp, w * h);

    show(result);
    return result;

}
int* Sandbox::sobel() {
    double SOBEL_X[] =  {
        1.0, 0, -1.0,
        2.0, 0, -2.0,
        1.0, 0, -1.0
    };
    double SOBEL_Y[] =  {
        1.0, 2.0, 1.0,
        0.0, 0.0, 0.0,
        -1.0, -2.0, -1.0
    };
    int size = 3;
    ConvolutionalTool* tool = new SequentialConvolutionalTool(imagePixmap.width(),
                                                              imagePixmap.height(),
                                                              SOBEL_X,
                                                              size,
                                                              SOBEL_Y);
    DataRetriver dr = DataRetriver(NULL);

    int* data = dr.retriveData(imagePixmap);
    int * result = tool->process(BORDER, GRAY, data);
    show(result);

    return result;
}
int* Sandbox::moravek(int winSize, int nPoints) {
    int w = imagePixmap.width();
    int h = imagePixmap.height();
    // do not display gauss
    setShowResultsFlagTo(false);
    gaussBlurGray(1.3);
    ImageToProcess toProcess = ImageToProcess();
    // gray, w* h, 0.0 ... 1.0
    toProcess.setDoubles(tool->getCanals(), w, h);

    setShowResultsFlagTo(true);
    double* temp = new double[w * h];
    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            double min = __DBL_MAX__;
            for (int dx = -1; dx <= 1; dx++) {
                for (int dy = -1; dy <= 1; dy++) {
                    if (dx != 0 && dy != 0) {
                        double c = toProcess.shiftError(winSize, i, j, dx, dy);
                        min = qMin(min, c);
                    }
                }
            }
            temp[i * w + j] = min;
        }
    }
    QList <PointOfInterest> points = toProcess.getPOIs(winSize);

    points = ImageToProcess::filterPOIs(&toProcess, points, nPoints);
    ImageToProcess doubleRgb = ImageToProcess(imagePixmap, R | G | B);


    // mark image
    foreach (PointOfInterest point, points) {
        doubleRgb.setValueSafe(point.getX() - 1, point.getY(), 1);
        doubleRgb.setValueSafe(point.getX() + 1, point.getY(), 1);
        doubleRgb.setValueSafe(point.getX(), point.getY(), 1);
        doubleRgb.setValueSafe(point.getX(), point.getY() - 1, 1);
        doubleRgb.setValueSafe(point.getX(), point.getY() + 1, 1);
    }
    int* result = Helper::toIntRGB(R | G | B, doubleRgb.doubleData , w * h);
    show(result);
    return result;
}
