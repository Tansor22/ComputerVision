#ifndef DESCRIPTOR_H
#include <helper.h>

#define DESCRIPTOR_H


class Descriptor
{
public:
    int nBaskets;   // baskets number per historam
    int nHistogram;
    QList<double> histograms = QList<double>();

public:
    Descriptor(int nBaskets, int nHistogram) : nBaskets(nBaskets), nHistogram(nHistogram) {
        Helper::init<double>(histograms, nBaskets * nHistogram * nHistogram);
    };
    Descriptor(): nBaskets(0), nHistogram(0) {};

    double getBasket (int hist, int basket) { return histograms[hist * nBaskets + basket];};
    void setBasket (int hist, int basket, double value) {histograms[hist * nBaskets + basket] = value;};
    QList<double> getHistograms() const {return  histograms;};
    void setHistograms(const QList<double> &value) {histograms = value;};
    int getBaskets() const {return nBaskets;};
    void setBaskets(int value) {nBaskets = value;};
    int getNHistogram() const {return nHistogram;};
    void setNHistogram(int value) {nHistogram = value;};

    // implemented outside
    QString asQString();
    void normalize();
};

#endif // DESCRIPTOR_H
