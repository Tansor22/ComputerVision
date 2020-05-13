#include "descriptor.h"

void Descriptor::normalize()
{
    double size = 0;
    for (double value : histograms)
        size += value * value;
    size = sqrt(size);

    for(int i = 0; i < histograms.size(); i++)
        if (histograms[i] / size > 0.2)
            histograms[i] = 0.2;
        else
            histograms[i] /= size;


    size = 0;
    for (double value : histograms)
        size += value * value;
    size = sqrt(size);

    for(int i = 0; i < histograms.size(); i++)
        histograms[i] /= size;
}

QString Descriptor::asQString() {
    QString output = QString();
    foreach(double histogram, histograms)
        output += " " + Helper::asQStringWithPrecision(histogram, 4);
    return output;
}
