#include "imagetoprocess.h"
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
                    reduced = ConvolutionalTool::reduce(canalsCount, toReduce, kernelW * kernelH);
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
    delete[] doubleData;
}

ImageToProcess::ImageToProcess(Canal type, double *data, int w, int h)
{
    setDoubles(type, Helper::copyOf(data, w * h), w, h);
}

ImageToProcess::ImageToProcess(QPixmap pixmap, Canal type) : type(type) {
    w = pixmap.width();
    h = pixmap.height();
    DataRetriver dr = DataRetriver();

    int* rgbs = dr.retriveData(pixmap);
    // remove second arg and gets values in 0.0 ... 255.0, otherwise 0.0 ... 1.0
    dr = DataRetriver(type, Helper::normalizeStraight);
    doubleData = dr.retriveData(rgbs, w, h);
}
int* ImageToProcess::toIntRGB() {
    return Helper::toIntRGB(type, doubleData, w * h);
}

QImage ImageToProcess::toQImage()
{
    int* rgbs = toIntRGB();
    QImage image = QImage(w, h, QImage::Format_RGB32);
    for (int y = 0; y < h; y++)
        for (int x = 0; x < w; x++)
            image.setPixelColor(x,y, rgbs[y * w + x]);
    return image;
}

void ImageToProcess::gaussBlur(double sigma) {
    int size = floor(3 * sigma);
    double* kernel =  Helper::gauss(sigma);

    ConvolutionalTool* tool = new SequentialConvolutionalTool(w, h, kernel, size);

    int* data = toIntRGB();
    // assert result == toIntRGB()
    int * result = tool->process(BORDER, type, data);
    setDoubles(type, Helper::copyOf(tool->getCanals(), w * h * Helper::canalsCount(type)), w, h);
    delete data;
    delete result;
    delete tool;
}

void ImageToProcess::derivativeX()
{
    double SOBEL_X[] =  {
        -1.0, 0, 1.0,
        -2.0, 0, 2.0,
        -1.0, 0, 1.0
    };
    int size = 3;

    ConvolutionalTool* tool =
            new SequentialConvolutionalTool(w, h, SOBEL_X, size);
    tool->setSobelFlagTo(true);

    // probably should transfer to 0 .. 255
    int* data = toIntRGB();
    tool->process(BORDER, GRAY, data);
    // probably should not copy
    double* derivativeX = Helper::copyOf(tool->getCanals(), w * h);
    // may return, not set
    setDoubles(type, derivativeX, w, h);
}

void ImageToProcess::derivativeY()
{
    double SOBEL_Y[] =  {
        -1.0, -2.0, -1.0,
        0.0, 0.0, 0.0,
        1.0, 2.0, 1.0
    };
    int size = 3;

    ConvolutionalTool* tool =
            new SequentialConvolutionalTool(w, h, SOBEL_Y, size);
    tool->setSobelFlagTo(true);

    // probably should transfer to 0 .. 255
    int* data = toIntRGB();
    tool->process(BORDER, GRAY, data);
    // probably should not copy
    double* derivativeX = Helper::copyOf(tool->getCanals(), w * h);
    // may return, not set
    setDoubles(type, derivativeX, w, h);
}

void ImageToProcess::gradient()
{
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
    ConvolutionalTool* tool = new SequentialConvolutionalTool(w,
                                                              h,
                                                              SOBEL_X,
                                                              size,
                                                              SOBEL_Y);

    int* data = toIntRGB();
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
    // coorect out of bound
    int i = y <= 0 ? 0 : (y >= h - 1 ? h - 1 : y);
    int j = x <= 0 ? 0 : (x >= w - 1 ? w - 1 : x);
    return doubleData[i * w + j];
}

void ImageToProcess::setValueSafe(int x, int y, double value) {
    // coorect out of bound
    if (x >= 0 && x < w && y > 0 && y < h){
        doubleData[y * w + x] = value;
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
QList<PointOfInterest> ImageToProcess::getPOIs(ImageToProcess* img,  int winSize, bool isHarris) {
    QList<PointOfInterest> points;

    int w = img->w;
    int h = img->h;
    int halfSize = winSize / 2;

    // that's intentional
    double min = __DBL_MAX__, max = __DBL_MIN__;
    for (int j = 0; j < h; j++) {
        for (int i = 0; i < w; i++) {
            double pixelValue = img->getValueSafe(i, j);
            if (max < pixelValue)
                max = pixelValue;
            if (min > pixelValue)
                min = pixelValue;
        }
    }

    // specify threshold
    double threshold = min + (max - min) * 0.005;
    if (isHarris)
        threshold = min + (max - min) * 0.004;
    //qDebug() << min << " " << max << " " << threshold;

    // the most powerful points will be added
    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            bool is_correct = true;
            double sLocal = img->getValueSafe(i, j);
            for (int px = -halfSize; px <= halfSize && is_correct; px++) {
                for (int py = -halfSize; py <= halfSize && is_correct; py++) {
                    if (px != 0 || py != 0) {
                        is_correct = sLocal > img->getValueSafe(i + px, j + py);
                    }
                }
            }
            if (is_correct && sLocal > threshold) {
                points.append(PointOfInterest(i, j, sLocal));;
            }
        }
    }
    return points;
}

QList<PointOfInterest> ImageToProcess::getPOIs(int winSize, bool isHarris) {
    return getPOIs(this, winSize, isHarris);
}
QList<PointOfInterest> ImageToProcess::filterPOIs(QList<PointOfInterest> pointsIn, int count) {
    return filterPOIs(this->w, this->h, pointsIn, count);
}
QList<PointOfInterest> ImageToProcess::filterPOIs(int w, int h, QList<PointOfInterest> pointsIn, int count)
{
    QList<PointOfInterest> points (pointsIn);

    int r = 1;

    // while the amount of points is sufficient and radius is ok
    while (points.size() > count && r < w / 2 && r < h / 2) {
        points.erase(std::remove_if(points.begin(), points.end(),
                                    [&](const PointOfInterest& curPoint) {
            for (const PointOfInterest& point : points) {
                double dst = PointOfInterest::distance(point, curPoint);
                // points quite close and the second one is more powerfull
                if (dst < r && curPoint.getC() < point.getC()) {
                    return true;
                }
            }
            return false;
        }), points.end());
        r++;
    }
    return points;
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

void ImageToProcess::downsample() {
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

