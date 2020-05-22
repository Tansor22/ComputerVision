#ifndef DESCRIPTOR_H
#include <helper.h>

#define DESCRIPTOR_H


class Descriptor
{
public:
    int nBaskets;   // baskets number per historam
    int nHistograms;
    QList<double> histograms = QList<double>();

public:
    Descriptor(int nBaskets, int nHistograms);
    Descriptor(): nBaskets(0), nHistograms(0) {};

    double getBasket (int hist, int basket) { return histograms[hist * nBaskets + basket];};
    void setBasket (int hist, int basket, double value) {histograms[hist * nBaskets + basket] = value;};
    QList<double> getHistograms() const {return  histograms;};
    void setHistograms(const QList<double> &value) {histograms = value;};
    int getBaskets() const {return nBaskets;};
    void setBaskets(int value) {nBaskets = value;};
    int getNHistograms() const {return nHistograms;};
    void setNHistograms(int value) {nHistograms = value;};
    double distance(Descriptor d);

    // implemented outside
    QString asQString();
    void normalize();
};

#endif // DESCRIPTOR_H
