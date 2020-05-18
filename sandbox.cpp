#include "imagetoprocess.h"
#include "sandbox.h"

#include <QApplication>

void Sandbox::show(int *pixels, int w, int h) {
    if (showResults && !innerCall) {
        if (pixels == 0) {
            // show current src image
            form->scene->addPixmap(imagePixmap);
        } else {
            // default params logic
            w = w < 0 ? imagePixmap.width() : w;
            h = h < 0 ? imagePixmap.height() : h;
            QImage image = QImage(w, h, QImage::Format_ARGB32);

            for (int y = 0; y < h; y++)
                for (int x = 0; x < w; x++)
                    image.setPixelColor(x,y, pixels[y * w + x]);
            form->scene->addPixmap(QPixmap::fromImage(image));
        }
        form->showMaximized();
    }
}

void Sandbox::show(ImageToProcess itp) {
    show(itp.toIntRGB(), itp.w, itp.h);
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
void Sandbox::write(ImageToProcess itp, QString fileName) {
    write(itp.toIntRGB(), fileName, itp.w, itp.h);

}
void Sandbox::write(int *pixels, QString fileName, int w, int h) {
    QImage image = imagePixmap.toImage();
    // default params logic
    w = w < 0 ? imagePixmap.width() : w;
    h = h < 0 ? imagePixmap.height() : h;
    for (int y = 0; y < h; y++)
        for (int x = 0; x < w; x++)
            image.setPixelColor(x,y, pixels[y * w + x]);

    QFile file(fileName + ".png");
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

double* Sandbox::getDoublesAs(Canal type, int w, int h, double (*mapper)(int))
{
    DataRetriver dr = DataRetriver(NULL);
    int* data = dr.retriveData(imagePixmap);
    dr = DataRetriver(type, mapper);
    // normalized
    return dr.retriveData(data, w, h);
}
//int* Sandbox::gaussBlurGrayV2(double sigma) {


//    int size = (int)(3 * sigma);
//    int halfSize = size / 2;
//    double ss2 = 2 * sigma * sigma;
//    double firstDrob = 1.0 / (M_PI * ss2);

//    double* tmp = new double[size];
//    for(int x = -halfSize,  ptr = 0; x <= halfSize ; x++){
//        double gauss = firstDrob * exp( -(x * x) / ss2);
//        tmp[ptr++] = gauss;

//    }

//    int w = imagePixmap.width();
//    int h = imagePixmap.height();

//    DataRetriver dr = DataRetriver(NULL);

//    int* data = dr.retriveData(imagePixmap);

//    dr = DataRetriver(GRAY, Helper::normalizeStraight);

//    double* rgbNormalized = dr.retriveData(data, w, h);

//    ImageToProcess itp = ImageToProcess();
//    itp.setDoubles(GRAY, rgbNormalized, w, h);
//    ConvolutionalTool* temp = new SequentialConvolutionalTool();
//    ImageToProcess output = temp->cross(&itp, tmp, size, 1);

//    ImageToProcess output2 = temp->cross(&output, tmp, 1, size);
//    int * result = output2.toIntRGB();

//    show(result);
//    return result;

//}
int* Sandbox::gaussBlurGray(double sigma) {
    int size = floor(3 * sigma);
    double* kernel =  Helper::gauss(sigma);

    tool = new SequentialConvolutionalTool(imagePixmap.width(),
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
    int * result = tool->process(BORDER, R | G | B, data);
    show(result);
    return result;
}
//int* Sandbox::crossDemo() {
//    double INCREASE_SHARPNESS[] =  {
//        -1.0, -1.0, -1.0,
//        -1.0, 9.0, -1.0,
//        -1.0, -1.0, -1.0
//    };

//    int size = 3;
//    int w = imagePixmap.width();
//    int h = imagePixmap.height();

//    DataRetriver dr = DataRetriver(NULL);

//    int* data = dr.retriveData(imagePixmap);

//    dr = DataRetriver(GRAY, Helper::normalizeStraight);

//    double* rgbNormalized = dr.retriveData(data, w, h);

//    ImageToProcess itp = ImageToProcess();
//    itp.setDoubles(GRAY, rgbNormalized, w, h);
//    ConvolutionalTool* temp = new SequentialConvolutionalTool();
//    ImageToProcess output = temp->cross(&itp, INCREASE_SHARPNESS, size, size);
//    int * result = output.toIntRGB();

//    show(result);
//    return result;
//}

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
ImageToProcess Sandbox::harris(int winSize, int nPoints) {
    int w = imagePixmap.width();
    int h = imagePixmap.height();

    ImageToProcess toProcess = ImageToProcess(imagePixmap, GRAY);
    toProcess.gaussBlur(1.3);

    // searching derivatives
    ImageToProcess dx = ImageToProcess(imagePixmap, GRAY), dy = ImageToProcess(imagePixmap, GRAY);
    dx.derivativeX();
    dy.derivativeY();
    dy.save("Y");
    dx.save("X");

    // derivatives have been found, proceeding
    double *a = new double [w * h];
    double *b = new double [w * h];
    double *c = new double [w * h];
    int halfSize = winSize / 2;


    // searching weighs for window
    double sigma = static_cast<double>(winSize) / 6;
    double *gaussKernel = new double[winSize * winSize];

    double coeff = 1 / (2 * M_PI * sigma * sigma);
    double devider = 2 * sigma * sigma;

    for (int u = -halfSize; u <= halfSize; u++)
        for (int v = -halfSize; v <= halfSize; v++)
            gaussKernel[(u + halfSize) * halfSize + (v  + halfSize)]
                    = coeff * exp(- (u * u + v * v) / devider);



    // calculating a,b,c for the points
    for (int j = 0; j < h; j++) {
        for (int i = 0; i < w; i++) {
            double sumA = 0, sumB = 0, sumC = 0;
            for (int u = -halfSize; u <= halfSize; u++) {
                for (int v = -halfSize; v <= halfSize; v++) {
                    double i_x = dx.getValueSafe(i + u, j + v);
                    double i_y = dy.getValueSafe(i + u, j + v);
                    sumA += i_x * i_x * gaussKernel[u * halfSize + v];
                    sumB += i_x * i_y * gaussKernel[u * halfSize + v];
                    sumC += i_y * i_y * gaussKernel[u * halfSize + v];
                }
            }
            a[j * w + i] = sumA;
            b[j * w + i] = sumB;
            c[j * w + i] = sumC;
        }
    }

    ImageToProcess tempItp = ImageToProcess(GRAY, w, h);


    // eigenvalues
    for (int j = 0; j < h; j++)
        for (int i = 0; i < w; i++) {
            double sc = a[j * w + i] + c[j * w + i];
            double d = a[j * w + i] * c[j * w + i] - b[j * w + i] * b[j * w + i];
            double det = sc * sc - 4 * d;
            double L1 = (sc + sqrt(det)) / 2;
            double L2 = (sc - sqrt(det)) / 2;
            double cHarris = qMin(L1, L2);
            tempItp.setValueSafe(i, j, cHarris);
        }


    pois = tempItp.getPOIs(5, true);


    tempItp.save("HARRIS_RESPOND");

    pois = ImageToProcess::filterPOIs(w, h, pois, nPoints);


    ImageToProcess toMark = ImageToProcess(imagePixmap, R | G | B);


    // mark image
    int crossSize = 3;
    foreach (PointOfInterest point, pois) {
        toMark.setValueSafe(point.getX(), point.getY(), 1);
        for (int i = 1; i <= crossSize; i++) {
            toMark.setValueSafe(point.getX() - i, point.getY(), 1);
            toMark.setValueSafe(point.getX() + i, point.getY(), 1);
            toMark.setValueSafe(point.getX(), point.getY() - i, 1);
            toMark.setValueSafe(point.getX(), point.getY() + i, 1);
        }
    }
    return toMark;
}

ImageToProcess Sandbox::moravek(int winSize, int nPoints) {
    int w = imagePixmap.width();
    int h = imagePixmap.height();
    // do not display gauss
    ImageToProcess toProcess = ImageToProcess(imagePixmap, GRAY);
    toProcess.gaussBlur(1.3);

    ImageToProcess temp = ImageToProcess(GRAY, w, h);
    for (int i = 0; i < h; i++)
        for (int j = 0; j < w; j++) {
            double min = __DBL_MAX__;
            for (int dx = -1; dx <= 1; dx++)
                for (int dy = -1; dy <= 1; dy++) {
                    if (dx != 0 && dy != 0) {
                        double c = toProcess.shiftError(winSize, i, j, dx, dy);
                        min = qMin(min, c);
                    }
                }
            // ?? i j
            temp.setValueSafe(j, i, min);
        }
    temp.save("MORAVEC_RESPONSE.png");
    QList <PointOfInterest> points = toProcess.getPOIs(winSize);

    // target points
    points = ImageToProcess::filterPOIs(w, h, points, nPoints);

    ImageToProcess toMark = ImageToProcess(imagePixmap, R | G | B);


    // return marked image
    int crossSize = 3;
    foreach (PointOfInterest point, points) {
        toMark.setValueSafe(point.getX(), point.getY(), 1.0);
        for (int i = 1; i <= crossSize; i++) {
            toMark.setValueSafe(point.getX() - i, point.getY(), 1.0);
            toMark.setValueSafe(point.getX() + i, point.getY(), 1.0);
            toMark.setValueSafe(point.getX(), point.getY() - i, 1.0);
            toMark.setValueSafe(point.getX(), point.getY() + i, 1.0);
        }
    }
    return toMark;
}
void Sandbox::calcPyramid(int nOctaves, int nLevels, double sigmaA, double sigma0) {
    double k = pow(2.0, 1.0 / (nLevels - 1)); // interval
    double sigmaB = sqrt(sigma0 * sigma0 - sigmaA * sigmaA);

    ImageToProcess* toProcess = new ImageToProcess(imagePixmap, R | G | B);

    // blur it a bit
    toProcess->gaussBlur(sigmaB);

    double sigma[nLevels - 1];
    double sigmaOld = sigma0;
    for (int i = 0; i < nLevels - 1; i++) {
        double sigmaNew = sigmaOld * k;
        sigma[i] = sqrt(sigmaNew * sigmaNew - sigmaOld * sigmaOld);
        sigmaOld = sigmaNew;
    }

    double sigmaEff = sigma0;
    QList <Pyramid*> *oneOctave;  // single octave
    Pyramid *currentLayer;

    for(int i = 0; i < nOctaves; i++){
        oneOctave = new QList <Pyramid*>();   //создаем новую октаву
        double sigmaLocal = sigma0;

        currentLayer = new Pyramid(toProcess, i, 0);
        currentLayer->setSigmaLocal(sigmaLocal);
        currentLayer->setSigmaEffective(sigmaEff);
        oneOctave->append(currentLayer);

        for (int j = 1; j < nLevels; j++){
            toProcess->gaussBlur(sigma[j - 1]);
            sigmaLocal *= k;
            sigmaEff *= k;
            currentLayer = new Pyramid(toProcess, i, j);
            currentLayer->setSigmaLocal(sigmaLocal);
            currentLayer->setSigmaEffective(sigmaEff);
            oneOctave->append(currentLayer);
        }
        pyramid.append(new Octave(oneOctave, i));

        if (i < nOctaves - 1)
            toProcess->downsample(); // shrinken image
    }

    // writting to hard disk
    foreach (Octave *octave, pyramid)
        foreach (Pyramid *layer, *octave->getLayers()) {
            QString path = "octave " + QString::number(layer->getOctaves() + 1) +
                    + "-sigma_local"+ QString::number(layer->getSigmaLocal()) + "-sigma_global" +QString::number(layer->getSigmaEffective()) +
                    "_image" + QString::number(layer->getLayers() + 1)+ ".png";
            layer->getImage()->toQImage().save(path);
        }
}

int* Sandbox::descriptors(int nPoints) {
    //    Descriptor d(2, 3);
    //    qDebug() << d.asQString();
    //    qDebug() << d.getHistograms();


    int w = imagePixmap.width();
    int h = imagePixmap.height();

    // getting pois with harris method
    harris(3, nPoints);
    ImageToProcess* itp = new ImageToProcess(GRAY, tool->getCanals(), w, h);
    DescriptorBuilder db (itp);

    return 0;
}
