//
// Created by Sergei on 23.05.2020.
//

#include "Contrast.h"

Contrast::Contrast(int _correction) : Distortion(), correction(_correction) {

}

QPixmap Contrast::distort(QPixmap pixmap) {
    int w = pixmap.width();
    int h = pixmap.height();
    int size = w * h;

    DataRetriever dr = DataRetriever();
    QRgb *rgbs = dr.retrieveData(pixmap);
    uchar buf[256];
    uint midBright = 0;
    uchar *pixels = new uchar[w * h * 3];
    int pixelPtr = 0;
    for (int i = 0; i < size; i++) {
        uchar r = qRed(rgbs[i]);
        uchar g = qGreen(rgbs[i]);
        uchar b = qBlue(rgbs[i]);
        midBright += r * 77 + g * 150 + b * 29;
        // target
        pixels[pixelPtr++] = r;
        pixels[pixelPtr++] = g;
        pixels[pixelPtr++] = b;
    }
    // average pixels brightness
    midBright /= (256 * size / 3);
    for (int i = 0; i < 256; i++) {
        int a = (((i - midBright) * correction) >> 8) + midBright;
        // clip
        if (a < 0) buf[i] = 0;
        else if (a > 255) buf[i] = 255;
        else buf[i] = a;
    }
    for (int i = 0; i < size; i++)
        pixels[i] = buf[pixels[i]];
    QPixmap output = QPixmap::fromImage(QImage(pixels, w, h,
                                               QImage::Format_RGB888));
    return output;
}
