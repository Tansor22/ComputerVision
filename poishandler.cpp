#include "poishandler.h"
// required to resolve circular dependency
#include "descriptorbuilder.h"
#include "imagetoprocess.h"
PoisHandler::PoisHandler()
{

}

QList<PointOfInterest> PoisHandler::getPOIs(ImageToProcess img,  int winSize, bool isHarris) {
    QList<PointOfInterest> pts;

    int w = img.w;
    int h = img.h;
    int halfSize = winSize / 2;

    // that's intentional
    double min = __DBL_MAX__, max = __DBL_MIN__;
    for (int i = 0; i < h; i++)
        for (int j = 0; j < w; j++) {
            double pixelValue = img.getValueSafe(j, i);
            if (max < pixelValue)
                max = pixelValue;
            if (min > pixelValue)
                min = pixelValue;
        }

    // specify threshold`
    double threshold = min + (max - min) * 0.005;
    if (isHarris)
        threshold *= 0.004;

    // the most powerful points will be added
    for (int i = 0; i < h; i++)
        for (int j = 0; j < w; j++) {
            bool isCorrect = true;
            double sLocal = img.getValueSafe(j, i);
            for (int px = -halfSize; px <= halfSize && isCorrect; px++)
                for (int py = -halfSize; py <= halfSize && isCorrect; py++)
                    if (px != 0 || py != 0)
                        // ?? j + py
                        //?? i + px
                        isCorrect = sLocal > img.getValueSafe(j + px, i + py);

            if (isCorrect && sLocal > threshold)
                pts << PointOfInterest(j, i, sLocal);

        }
    return pts;
}


QList<PointOfInterest> PoisHandler::filterPOIs(int w, int h, QList<PointOfInterest> pointsIn, int count)
{
    QList<PointOfInterest> pts (pointsIn);

    int r = 1;

    // while the amount of points is sufficient and radius is ok
    while (pts.size() > count && r < w / 2 && r < h / 2) {
        pts.erase(std::remove_if(pts.begin(), pts.end(),
                                 [&](const PointOfInterest& curPoint) {
            for (const PointOfInterest& point : pts) {
                double dst = PointOfInterest::distance(point, curPoint);
                // points quite close and the second one is more powerfull
                if (dst < r && curPoint.getC() < point.getC()) {
                    return true;
                }
            }
            return false;
        }), pts.end());
        r++;
    }
    return pts;
}

QList<PointOfInterest> PoisHandler::handle(PoisAlgo algo, ImageToProcess itp, int winSize, int nPoints, bool needDescriptors)
{
    switch(algo) {
    case PoisAlgo::MORAVEC: return handleMoravec(itp, winSize, nPoints, needDescriptors);
    case PoisAlgo::HARRIS: return handleHarris(itp, winSize, nPoints, needDescriptors);
    default: qDebug() << "Unknown Pois Algo" << endl;
    }
    return QList<PointOfInterest>();
}

QList<PointOfInterest> PoisHandler::handleMoravec(ImageToProcess itp, int winSize, int nPoints, bool needDescriptors)
{
    int w = itp.w;
    int h = itp.w;

    ImageToProcess temp = ImageToProcess(GRAY, w, h);
    for (int i = 0; i < h; i++)
        for (int j = 0; j < w; j++) {
            double min = __DBL_MAX__;
            for (int dx = -1; dx <= 1; dx++)
                for (int dy = -1; dy <= 1; dy++) {
                    if (dx != 0 && dy != 0) {
                        double c = itp.shiftError(winSize, i, j, dx, dy);
                        min = qMin(min, c);
                    }
                }
            temp.setValueSafe(j, i, min);
        }
    temp.save("MORAVEC_RESPONSE.png");
    QList <PointOfInterest> pois = getPOIs(itp, winSize);

    // target points
    pois = PoisHandler::filterPOIs(w, h, pois, nPoints);
    if (needDescriptors) {
        DescriptorBuilder db (&itp);
        pois = db.calcOrientPoints(pois);

        foreach (PointOfInterest poi, pois) {
            poi.setDescriptor(db.buildDescriptor(poi));
        }
    }
    return pois;
}

QList<PointOfInterest> PoisHandler::handleHarris(ImageToProcess itp, int winSize, int nPoints, bool needDescriptors)
{
    int w = itp.w;
    int h = itp.w;
    ImageToProcess dx = ImageToProcess(itp), dy = ImageToProcess(itp);

    // searching derivatives
    dx.derivativeX();
    dx.setName("dx");
    dy.derivativeY();
    dx.setName("dy");
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
    tempItp.setName("tempItp");


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


    QList <PointOfInterest> pois = getPOIs(tempItp, 5, true);


    tempItp.save("HARRIS_RESPOND");

    pois = PoisHandler::filterPOIs(w, h, pois, nPoints);

    if (needDescriptors) {
        DescriptorBuilder db (&itp);
        pois = db.calcOrientPoints(pois);

        foreach (PointOfInterest poi, pois) {
            poi.setDescriptor(db.buildDescriptor(poi));
        }
    }
    return pois;
}


