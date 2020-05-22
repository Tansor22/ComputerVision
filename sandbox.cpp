#include "imagehandler.h"
#include "sandbox.h"
#include <QApplication>
#include <utility>

Sandbox::Sandbox() {

}

void Sandbox::show(QRgb*pixels, int w, int h) const {
    if (showResults && !innerCall) {
        if (pixels == nullptr) {
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

void Sandbox::show(ImageToProcess* itp) const {
    show(itp->toIntRGB(), itp->w, itp->h);
}
void Sandbox::getImageViaFileName(const QString& fn)  {
    fileName = fn;
    imagePixmap.load(fn);
}

void Sandbox::getImageViaFileDialog() {

    fileName = QFileDialog::
            getOpenFileName(nullptr,
                            "Open Image", IMAGES_PATH, "Image Files (*.png *.jpg *.bmp)");
    imagePixmap.load(fileName);
}
void Sandbox::write(ImageToProcess itp, QString fn) const {
    write(itp.toIntRGB(), std::move(fn), itp.w, itp.h);
}

void Sandbox::write(QRgb* pixels, const QString& fn, int w, int h) const {
    QImage image = imagePixmap.toImage();
    // default params logic
    w = w < 0 ? imagePixmap.width() : w;
    h = h < 0 ? imagePixmap.height() : h;
    for (int y = 0; y < h; y++)
        for (int x = 0; x < w; x++)
            image.setPixelColor(x,y, pixels[y * w + x]);

    QFile file(fn + ".png");
    file.open(QIODevice::WriteOnly);
    image.save(&file, "PNG");
}

QRgb* Sandbox::grayScaled() const {
    DataRetriever dr = DataRetriever(0);

    QRgb* data = dr.retrieveData(imagePixmap);
    // such a retriever returns single channel(gray) 0..255 (no mapper) as double
    dr = DataRetriever(GRAY);
    double* grayScaled = dr.retrieveData(data, imagePixmap.width(), imagePixmap.height());
    int size = imagePixmap.width() * imagePixmap.height();
    data = new QRgb[size];
    for (int i = 0; i < size; i ++)
        data[i] = qRgb(static_cast<int>(grayScaled[i]), static_cast<int>(grayScaled[i]), static_cast<int>(grayScaled[i]));
    show(data);
    return data;
}

double* Sandbox::getDoublesAs(Canal type, int w, int h, double (*mapper)(QRgb)) const
{
    DataRetriever dr = DataRetriever();
    QRgb* data = dr.retrieveData(imagePixmap);
    dr = DataRetriever(type, mapper);
    // normalized
    return dr.retrieveData(data, w, h);
}

QRgb* Sandbox::gaussBlurGray(double sigma) const {
    int size = floor(3 * sigma);
    double* kernel =  KernelsHandler::getGauss(sigma);

    ConvolutionTool* tool = new SequentialConvolutionTool(imagePixmap.width(),
                                         imagePixmap.height(),
                                         kernel,
                                         size);

    DataRetriever dr = DataRetriever();

    QRgb* data = dr.retrieveData(imagePixmap);
    QRgb* result = tool->process(BORDER, GRAY, data);
    show(result);
    return result;
}
QRgb* Sandbox::gaussBlurRGB(double sigma) const {
    int size = floor(3 * sigma);
    double* kernel =  Helper::gauss(sigma);

    ConvolutionTool* tool = new ParallelConvolutionTool(imagePixmap.width(),
                                                        imagePixmap.height(),
                                                        kernel,
                                                        size);

    DataRetriever dr = DataRetriever();

    QRgb* data = dr.retrieveData(imagePixmap);
    QRgb* result = tool->process(BORDER, R | G | B, data);
    show(result);
    return result;
}

QRgb* Sandbox::increaseSharpness() const {
    ConvolutionTool* tool = new ParallelConvolutionTool(imagePixmap.width(),
                                                        imagePixmap.height(),
                                                        KernelsHandler::getIncreaseSharpness(),
                                                        3);

    DataRetriever dr = DataRetriever();

    QRgb* data = dr.retrieveData(imagePixmap);
    QRgb* result = tool->process(BORDER, R | G | B | A, data);
    show(result);
    return result;
}
QRgb* Sandbox::sobelV2() const {
    int w = imagePixmap.width();
    int h = imagePixmap.height();
    int size = 3;
    ConvolutionTool* tool = new SequentialConvolutionTool(w, h,  KernelsHandler::getSobelX(), size);
    tool->setSobelFlagTo(true);
    DataRetriever dr = DataRetriever();

    QRgb* data = dr.retrieveData(imagePixmap);
    // ignoring the result
    tool->process(BORDER, GRAY, data);
    double* derivativeX = Helper::copyOf(tool->getCanals(), w * h);

    // y derivative
    tool->recharge(KernelsHandler::getSobelX(), size);
    // ignoring the result
    tool->process(BORDER, GRAY, data);
    // no need to copy
    double* derivativeY = tool->getCanals();

    double* temp = new double[w * h];
    QRgb* result;

    for (int i = 0; i < w * h; i++)
        temp[i] = sqrt(derivativeY[i] * derivativeY[i] +
                       derivativeX[i] * derivativeX[i]);

    dr = DataRetriever(GRAY);
    dr.normalizeExtra(w *h, temp);

    result = Helper::toIntRGB(GRAY, temp, w * h);

    show(result);
    return result;

}
QRgb* Sandbox::sobel() const {
    ConvolutionTool* tool = new SequentialConvolutionTool(imagePixmap.width(),
                                                          imagePixmap.height(),
                                                          KernelsHandler::getSobelX(),
                                                          3,
                                                          KernelsHandler::getSobelY());
    tool->setSobelFlagTo(true);
    DataRetriever dr = DataRetriever();

    QRgb* data = dr.retrieveData(imagePixmap);
    QRgb* result = tool->process(BORDER, GRAY, data);
    show(result);

    return result;
}
ImageToProcess* Sandbox::harris(int winSize, int nPoints) const {
    ImageToProcess* toProcess = new ImageToProcess(imagePixmap, GRAY);
    // blur it a bit
    toProcess->gaussBlur(1.3);

    QList <PointOfInterest> pois =
            PoisHandler::handle(PoisHandler::HARRIS, toProcess, winSize, nPoints);


    ImageToProcess* toMark = new ImageToProcess(imagePixmap, R | G | B);


    // mark image
    ImageHandler::markWithWhite(toMark, pois);
    return toMark;
}

ImageToProcess* Sandbox::moravec(int winSize, int nPoints) const {
    ImageToProcess* toProcess = new ImageToProcess(imagePixmap, GRAY);
    // blur it a bit
    toProcess->gaussBlur(1.3);

    QList <PointOfInterest> pois =
            PoisHandler::handle(PoisHandler::MORAVEC, toProcess, winSize, nPoints);


    ImageToProcess* toMark = new ImageToProcess(imagePixmap, R | G | B);

    // return marked image
    ImageHandler::markWithWhite(toMark, pois);
    return toMark;
}
void Sandbox::calcPyramid(int nOctaves, int nLevels, double sigmaA, double sigma0) {
    double k = pow(2.0, 1.0 / (nLevels - 1)); // interval
    double sigmaB = sqrt(sigma0 * sigma0 - sigmaA * sigmaA);

    auto* toProcess = new ImageToProcess(imagePixmap, R | G | B);

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
        oneOctave = new QList <Pyramid*>();
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
            toProcess->downSample(); // shrinking image
    }

    // writing to hard disk
    foreach (Octave *octave, pyramid)
        foreach (Pyramid *layer, *octave->getLayers()) {
            QString path = "octave " + QString::number(layer->getOctaves() + 1) +
                    + "-sigma_local"+ QString::number(layer->getSigmaLocal()) + "-sigma_global" +QString::number(layer->getSigmaEffective()) +
                    "_image" + QString::number(layer->getLayers() + 1)+ ".png";
            layer->getImage()->toQImage().save(path);
        }
}

QRgb* Sandbox::descriptors(int nPoints, Distortion* distortion) {
    ImageToProcess *itp = new ImageToProcess(imagePixmap, R | G | B);
    itp->setName("Target");
    QList <PointOfInterest> itpPois =
            PoisHandler::handle(PoisHandler::HARRIS, itp, 3, nPoints, true);

    // getting not rotated
    //getImageViaFileName(fileName);

    //assert(itpPois.size() > 0  && "itpPois is empty");
    qDebug() << "tr";
    // mutate imagePixmap, further addressing to imagePixmap would be to rotated
    //getImageViaFileName(fileName);
    imagePixmap = distortion->distort(imagePixmap);


    ImageToProcess* distorted = new ImageToProcess(imagePixmap, R | G | B);
    QList <PointOfInterest> distortedPois =
            PoisHandler::handle(PoisHandler::HARRIS, distorted, 3, nPoints, true);
    assert(distortedPois.size() > 0 && "distortedPois is empty");
    // distances between descriptors of images
    auto *distances = new double[itpPois.size() * distortedPois.size()];

    // that's intentional
    auto min = __DBL_MAX__;
    auto max = __DBL_MIN__;
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

    QImage joined = ImageHandler::join(itp, distorted);

    QPainter p (&joined);
    p.setPen(QColor(255, 255, 255, 160));
    // mark source image with its pois
    ImageHandler::markWithWhite(itp, itpPois);
    // mark distorted image with its pois
    ImageHandler::markWithWhite(distorted, distortedPois);

    // matching descriptors
    for(int i = 0; i < itpPois.size(); i++) {
        auto firstMin = __DBL_MAX__;
        int firstMinI = 0;
        auto secondMin = __DBL_MAX__;

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
        auto firstMin2 = __DBL_MAX__;
        auto secondMin2 = __DBL_MAX__;
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
        double T = .8;
        if (NNDR < T && NNDR2 < T && firstMin < max * 0.3) {
            qDebug() << "mark";
            // match
            QRgb rgb = Helper::supplyWithRGB();
            int crossSize = 3;
            // source image
            itp->setValueSafe(itpPois[i].getX(), itpPois[i].getY(), rgb);
            for (int m = 1; m <= crossSize; m++) {
                itp->setValueSafe(itpPois[i].getX() - m,
                                 itpPois[i].getY(), rgb);
                itp->setValueSafe(itpPois[i].getX() + m,
                                 itpPois[i].getY(), rgb);
                itp->setValueSafe(itpPois[i].getX(),
                                 itpPois[i].getY() - m, rgb);
                itp->setValueSafe(itpPois[i].getX(),
                                 itpPois[i].getY() + m, rgb);
            }
            // rotated image
            distorted->setValueSafe(distortedPois[i].getX(), distortedPois[i].getY(), rgb);
            for (int m = 1; m <= crossSize; m++) {
                distorted->setValueSafe(distortedPois[firstMin].getX() - m,
                                     distortedPois[firstMin].getY(), rgb);
                distorted->setValueSafe(distortedPois[firstMin].getX() + m,
                                     distortedPois[firstMin].getY(), rgb);
                distorted->setValueSafe(distortedPois[firstMin].getX(),
                                       distortedPois[firstMin].getY() - m, rgb);
                distorted->setValueSafe(distortedPois[firstMin].getX(),
                                       distortedPois[firstMin].getY() + m, rgb);
            }
            // draw a link
            p.drawLine(itpPois[i].getX(), itpPois[i].getY(),
                       distortedPois[firstMin].getX() + itp->getW(), distortedPois[firstMin].getY());
        }
        // matching ends
    }
     // save results
    joined.save("JOINED.jpg", "JPG");
    itp->save("SOURCE");
    distorted->save("ROTATED");

    qDebug() << "end descriptors" << endl;
    return 0;
}
