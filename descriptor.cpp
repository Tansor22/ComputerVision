#include "descriptor.h"

Descriptor::Descriptor(int nBaskets, int nHistogram): nBaskets(nBaskets), nHistograms(nHistogram) {
    Helper::init<double>(histograms, nBaskets * nHistogram * nHistogram);
}

double Descriptor::distance(Descriptor d)
{
    double sum = 0;
    for(int i = 0; i < nHistograms; i++){
        for(int j = 0; j < nBaskets; j++){
            sum += pow((getBasket(i, j) - d.getBasket(i,j)), 2);
        }
    }
    return sqrt(sum);
}

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
