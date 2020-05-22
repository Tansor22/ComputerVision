#include "imagetoprocess.h"

#include <utility>

void ImageToProcess::cross(double* kernel, int kernelW, int kernelH, double divider) {
    // data required by calculation
    int ku = kernelH / 2;
    int kv = kernelW / 2;
    int canalsCount = Helper::canalsCount(type);

    double* crossed = new double[w * h * canalsCount];

    // initialize auxiliary data
    double* toReduce = new double[kernelW * kernelH * canalsCount];

    for (int i = 0; i < h; i++)
        for (int j = 0; j < w; j++) {
            double* reduced;
            // cross
            for(int u = -ku ; u <= ku; u++)
                for(int v = -kv ; v <= kv; v++){
                    int x = i - u;
                    int y = j - v;
                    // correct out of bound, copies of the image at the edges effect
                    if(x < 0 || y < 0 || x >= h || y >= w){
                        // x coordinat
                        if( x < 0)
                            x += h;
                        else if( x >= h)
                            x -= h;
                        // y coordinat
                        if( y < 0)
                            y += w;
                        else if( y >= w)
                            y -= w;
                    }
                    // store values to reduce by canals, canal-corrected ->  + w * h * c
                    int reducedPtr = 0;
                    for (int c = 0; c < canalsCount; c++)
                        toReduce[reducedPtr++] =
                                kernel[(u + ku) * kernelW + v + kv]* doubleData[x * w + y + w * h * c];
                    reduced = ConvolutionTool::reduce(canalsCount, toReduce, kernelW * kernelH);
                }
            //Helper::printSample(0, canalsCount, reduced);

            // correct by divider and fill output array
            for (int c = 0; c < canalsCount; c++) {
                reduced[c] *= divider;
                crossed[i * w + j + w * h * c] = reduced[c];
            }

        }
    setDoubles(type, crossed, w, h);
}
ImageToProcess::ImageToProcess()
{

}

ImageToProcess::~ImageToProcess()
{
    //delete[] doubleData;
}

ImageToProcess::ImageToProcess(Canal type, double *data, int w, int h)
{
    setDoubles(type, Helper::copyOf(data, w * h), w, h);
}

ImageToProcess::ImageToProcess(QPixmap pixmap, Canal type) : type(type) {
    w = pixmap.width();
    h = pixmap.height();
    DataRetriever dr = DataRetriever();

    QRgb* rgbs = dr.retrieveData(pixmap);
    // remove second arg and gets values in 0.0 ... 255.0, otherwise 0.0 ... 1.0
    dr = DataRetriever(type, Helper::normalizeStraight);
    doubleData = dr.retrieveData(rgbs, w, h);
}

ImageToProcess::ImageToProcess(ImageToProcess *itp, OutOfBoundPolicy oobp) : oobp(oobp)
{
    setDoubles(itp->type,
               Helper::copyOf(itp->doubleData,
                              itp->w * itp->h * Helper::canalsCount(itp->type))
               , itp->w, itp->h);
}

ImageToProcess::ImageToProcess(Canal type, int w, int h) : type(type), w(w), h(h) {
    doubleData = Helper::getZeroFilledArr(w * h * Helper::canalsCount(type));
}
QRgb* ImageToProcess::toIntRGB() {
    return Helper::toIntRGB(type, doubleData, w * h);
}

QImage ImageToProcess::toQImage()
{
    QRgb* rgbs = toIntRGB();
    QImage image = QImage(w, h, QImage::Format_RGB32);
    for (int y = 0; y < h; y++)
        for (int x = 0; x < w; x++)
            image.setPixelColor(x,y, rgbs[y * w + x]);
    return image;
}

void ImageToProcess::gaussBlur(double sigma) {
    int size = floor(3 * sigma);
    double* kernel =  Helper::gauss(sigma);

    ConvolutionTool* tool = new SequentialConvolutionTool(w, h, kernel, size);

    QRgb*  data = toIntRGB();
    // assert result == toIntRGB()
    QRgb*  result = tool->process(BORDER, type, data);
    setDoubles(type, Helper::copyOf(tool->getCanals(), w * h * Helper::canalsCount(type)), w, h);
    delete data;
    delete result;
    delete tool;
}

void ImageToProcess::derivativeX()
{
    ConvolutionTool* tool =
            new SequentialConvolutionTool(w, h, KernelsHandler::getSobelX(), 3);
    tool->setSobelFlagTo(true);

    // probably should transfer to 0 .. 255
    QRgb* data = toIntRGB();
    tool->process(BORDER, GRAY, data);
    // probably should not copy
    double* derivativeX = Helper::copyOf(tool->getCanals(), w * h);
    // may return, not set
    setDoubles(type, derivativeX, w, h);
    delete tool;
}

void ImageToProcess::save(QString fileName)
{
    toQImage().save(fileName + ".jpg", "JPG");
}

void ImageToProcess::derivativeY()
{
    ConvolutionTool* tool =
            new SequentialConvolutionTool(w, h, KernelsHandler::getSobelY(), 3);
    tool->setSobelFlagTo(true);

    // probably should transfer to 0 .. 255
    QRgb*  data = toIntRGB();
    tool->process(BORDER, GRAY, data);
    // probably should not copy
    double* derivativeX = Helper::copyOf(tool->getCanals(), w * h);
    // may return, not set
    setDoubles(type, derivativeX, w, h);
    delete tool;
}

void ImageToProcess::gradient()
{
    ConvolutionTool* tool =
            new SequentialConvolutionTool(w, h, KernelsHandler::getSobelX(), 3, KernelsHandler::getSobelY());
    tool->setSobelFlagTo(true);
    QRgb* data = toIntRGB();
    tool->process(BORDER, GRAY, data);
    setDoubles(type, tool->getCanals(), w, h);
}

void ImageToProcess::setDoubles(Canal type, double *doubleData, int w, int h) {
    this->doubleData = doubleData;
    this->type = type;
    this->w = w;
    this->h = h;
}
double ImageToProcess::getValueSafe(int x, int y) {
    // correct out of bound
    switch (oobp) {
        case EDGE: {
            int i = y <= 0 ? 0 : (y >= h - 1 ? h - 1 : y);
            int j = x <= 0 ? 0 : (x >= w - 1 ? w - 1 : x);
            return doubleData[i * w + j];
        }
        case BLACK: {
            return (x >= 0 && x <= w - 1 && y >= 0 && y <= h - 1) ? doubleData[y * w + x] : 0.0;
            }
        case WHITE: {
            return (x >= 0 && x <= w - 1 && y >= 0 && y <= h - 1) ? doubleData[y * w + x] : 1.0;
        }
        case SPREAD: {
            int i = y, j = x;
            // x coordinat
            if( x < 0)
                j += h;
            else if( x >= h)
                j -= h;
            // y coordinat
            if( y < 0)
                i += w;
            else if( y >= w)
                i -= w;
            return doubleData[i * w + j];

        }
    }
}

// for debug
double ImageToProcess::operator[](int i) const
{
    return doubleData[i];
}

void ImageToProcess::setValueSafe(int x, int y, double value) {
    // coorect out of bound
    if (x >= 0 && x < w && y > 0 && y < h){
        // for none single canal images
        for (int c = 0; c < Helper::canalsCount(type); c++)
            doubleData[(y * w + x) + w * h * c] = value;
    }
}

void ImageToProcess::setValueSafe(int x, int y, QRgb rgb)
{
    double* doubleValues = Helper::getDoublesTupleForCanals(type, rgb);
    // coorect out of bound
    if (x >= 0 && x < w && y > 0 && y < h){
        // for none single canal images
        for (int c = 0; c < Helper::canalsCount(type); c++)
            doubleData[(y * w + x) + w * h * c] = doubleValues[c];
    }
}

double ImageToProcess::shiftError(int winSize, int x, int y, int dx, int dy) {
    double sum = 0;
    auto winSizeHalf = winSize / 2;

    for (int u = -winSizeHalf; u <= winSizeHalf; u++) {
        for (int v = -winSizeHalf; v <= winSizeHalf; v++) {
            double tmp =  getValueSafe(x + u, y + v) - getValueSafe(x + u + dx, y + v + dy);
            sum += tmp * tmp;
        }
    }
    return sum;
}

QList<PointOfInterest> ImageToProcess::getPOIs(int winSize, bool isHarris) {
    return PoisHandler::getPOIs(this, winSize, isHarris);
}
QList<PointOfInterest> ImageToProcess::filterPOIs(QList<PointOfInterest> pointsIn, int count) {
    // ??? move
    return PoisHandler::filterPOIs(this->w, this->h, std::move(pointsIn), count);
}

double *ImageToProcess::getDoubles() const
{
    return doubleData;
}

int ImageToProcess::getH() const
{
    return h;
}

int ImageToProcess::getW() const
{
    return w;
}

void ImageToProcess::downSample() {
    // only for rgb
    if (Helper::isGray(type))
        return;

    int nCanals = Helper::noAlpha(type) ? 3 : 4;

    int oldW = w;
    int oldH = h;
    w = w / 2;
    h = h / 2;
    int size = w * h;
    int oldSize = oldW * oldH;


    double * oldDoubleData = doubleData;

    doubleData = new double[size * nCanals];

    for (int i = 0; i < h; i++)
        for (int j = 0; j < w; j++)
            for (int c = 0; c < nCanals; c++)
                // seems spooky, doesn't it?
                doubleData[i * w + j + size * c]
                        = oldDoubleData[i * 2 * oldW + j * 2 + oldSize * c];

    delete [] oldDoubleData;

}

void ImageToProcess::setName(const QString &value)
{
    name = value;
}

Canal ImageToProcess::getType() const
{
    return type;
}

void ImageToProcess::setType(const Canal &value)
{
    type = value;
}

