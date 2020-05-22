#include "imagehandler.h"
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
    ImageToProcess toProcess = ImageToProcess(imagePixmap, GRAY);
    // blur it a bit
    toProcess.gaussBlur(1.3);

    QList <PointOfInterest> pois =
            PoisHandler::handle(PoisHandler::HARRIS, toProcess, winSize, nPoints);


    ImageToProcess toMark = ImageToProcess(imagePixmap, R | G | B);


    // mark image
    ImageHandler::markWithWhite(toMark, pois);
    return toMark;
}

ImageToProcess Sandbox::moravek(int winSize, int nPoints) {
    ImageToProcess toProcess = ImageToProcess(imagePixmap, GRAY);
    // blur it a bit
    toProcess.gaussBlur(1.3);

    QList <PointOfInterest> pois =
            PoisHandler::handle(PoisHandler::MORAVEC, toProcess, winSize, nPoints);


    ImageToProcess toMark = ImageToProcess(imagePixmap, R | G | B);


    // return marked image
    ImageHandler::markWithWhite(toMark, pois);
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
    ImageToProcess itp = new ImageToProcess(imagePixmap, GRAY);
    itp.setName("outer");
    QList <PointOfInterest> itpPois =
            PoisHandler::handle(PoisHandler::HARRIS, itp, 3, nPoints, true);

    // getting not rotated
    //getImageViaFileName(fileName);

    // mutate imagePixmap, futher addressing to imagePixmap would be to rotated
    imagePixmap = ImageHandler::distort(imagePixmap, ImageHandler::IDENTITY, 15);
//        QMatrix rm;
//        rm.rotate(90);
//        imagePixmap = imagePixmap.transformed(rm);
        QPixmap rotatedPixmap = ImageHandler::rotate(imagePixmap, 15);

    ImageToProcess distorted = ImageToProcess(imagePixmap, GRAY);
    QList <PointOfInterest> distortedPois =
            PoisHandler::handle(PoisHandler::HARRIS, distorted, 3, nPoints, true);
    // distances between descriptors of images
    double *distances = new double[itpPois.size() * distortedPois.size()];

    // that's intentional
    double min = __DBL_MAX__;
    double max = __DBL_MIN__;
    double mid = 0;

    for(int i = 0; i < itpPois.size(); i++)
        for(int j = 0; j < distortedPois.size(); j++){
            double distance = itpPois[i].getDescriptor()
                    .distance(distortedPois[i].getDescriptor());
            distances[i * distortedPois.size() + j] = distance;
            mid += distance;
            if(distance < min)
                min = distance;

            if(distance > max)
                max = distance;
        }


    mid /= itpPois.size() * distortedPois.size();
    //qDebug()<<"MinDistance: " << min <<" MaxDistance: " << max << "mid: " << mid;

    QImage joined = ImageHandler::join(itp, distorted);
//    QPixmap px(itp.getW() + rotated.getW(), qMax(itp.getH(), rotated.getH()));
//    QPainter pJoin(&px);
//    pJoin.drawImage(0, 0, itp.toQImage());
//    pJoin.drawImage(itp.getW(), 0, rotated.toQImage());
//    QImage joined = px.toImage();
    QPainter p (&joined);
    p.setPen(QColor(255, 255, 255, 160));
    // mark source image with its pois
    ImageHandler::markWithWhite(itp, itpPois);
    // mark distorted image with its pois
    ImageHandler::markWithWhite(distorted, distortedPois);

    // matching descriptors
    for(int i = 0; i < itpPois.size(); i++) {
        double firstMin = __DBL_MAX__;
        int firstMinI = 0;
        double secondMin = __DBL_MAX__;

        for(int j = 0; j < distortedPois.size(); j++){
            double distance = distances[i * distortedPois.size() + j];
            if(distance < firstMin){
                secondMin = firstMin;
                firstMin = distance;
                firstMinI = j;
            } else {
                if(distance < secondMin)
                    secondMin = distance;
            }
        }
        // matching descriptors continues
        double firstMin2 = __DBL_MAX__;
        double secondMin2 = __DBL_MAX__;
        for(int j = 0; j < itpPois.size(); j++) {
            double distance = distances[j * distortedPois.size() + firstMinI];
            if(distance < firstMin2){
                secondMin2 = firstMin2;
                firstMin2 = distance;
            } else {
                if(distance < secondMin2)
                    secondMin2 = distance;
            }
        }
        // matching descriptors continues
        // discard according to NNDR
        double NNDR = firstMin / secondMin;
        double NNDR2 = firstMin2 / secondMin2;
        // TODO play on param
        double T = .6;
        if (NNDR < T && NNDR2 < T && firstMin < max * 0.3) {
            // match
            QRgb rgb = Helper::supplyWithRGB();
            int crossSize = 3;
            // source image
            itp.setValueSafe(itpPois[i].getX(), itpPois[i].getY(), rgb);
            for (int i = 1; i <= crossSize; i++) {
                itp.setValueSafe(itpPois[i].getX() - i,
                                 itpPois[i].getY(), rgb);
                itp.setValueSafe(itpPois[i].getX() + i,
                                 itpPois[i].getY(), rgb);
                itp.setValueSafe(itpPois[i].getX(),
                                 itpPois[i].getY() - i, rgb);
                itp.setValueSafe(itpPois[i].getX(),
                                 itpPois[i].getY() + i, rgb);
            }
            // rotated image
            distorted.setValueSafe(distortedPois[i].getX(), distortedPois[i].getY(), rgb);
            for (int i = 1; i <= crossSize; i++) {
                distorted.setValueSafe(distortedPois[firstMin].getX() - i,
                                     distortedPois[firstMin].getY(), rgb);
                distorted.setValueSafe(distortedPois[firstMin].getX() + i,
                                     distortedPois[firstMin].getY(), rgb);
                distorted.setValueSafe(distortedPois[firstMin].getX(),
                                     distortedPois[firstMin].getY() - i, rgb);
                distorted.setValueSafe(distortedPois[firstMin].getX(),
                                     distortedPois[firstMin].getY() + i, rgb);
            }
            // draw a link
            p.drawLine(itpPois[i].getX(), itpPois[i].getY(),
                       distortedPois[firstMin].getX() + itp.getW(), distortedPois[firstMin].getY());
        }
        // matching ends
    }
     // save results
    joined.save("JOINED.jpg", "JPG");
    itp.save("SOURCE");
    distorted.save("ROTATED");

    qDebug() << "end descriptors" << endl;
    return 0;
}
