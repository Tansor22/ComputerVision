#include "imagehandler.h"

ImageHandler::ImageHandler()
{

}

QImage ImageHandler::join(ImageToProcess o1, ImageToProcess o2)
{
    QPixmap px(o1.getW() + o2.getW(), qMax(o1.getH(), o2.getH()));
    QPainter p(&px);
    p.drawImage(0, 0, o1.toQImage());
    p.drawImage(o1.getW(), 0, o2.toQImage());
    return px.toImage();
}

void ImageHandler::markWithWhite(ImageToProcess &itp, QList<PointOfInterest> pois, int crossSize)
{
    foreach (PointOfInterest point, pois) {
        itp.setValueSafe(point.getX(), point.getY(), 1.0);
        for (int i = 1; i <= crossSize; i++) {
            itp.setValueSafe(point.getX() - i, point.getY(), 1.0);
            itp.setValueSafe(point.getX() + i, point.getY(), 1.0);
            itp.setValueSafe(point.getX(), point.getY() - i, 1.0);
            itp.setValueSafe(point.getX(), point.getY() + i, 1.0);
        }
    }
}

void ImageHandler::markWithRGB(ImageToProcess &itp, QList<PointOfInterest> pois, QRgb rgb, int crossSize)
{
    foreach (PointOfInterest point, pois) {
        itp.setValueSafe(point.getX(), point.getY(), rgb);
        for (int i = 1; i <= crossSize; i++) {
            itp.setValueSafe(point.getX() - i, point.getY(), rgb);
            itp.setValueSafe(point.getX() + i, point.getY(), rgb);
            itp.setValueSafe(point.getX(), point.getY() - i, rgb);
            itp.setValueSafe(point.getX(), point.getY() + i, rgb);
        }
    }
}
