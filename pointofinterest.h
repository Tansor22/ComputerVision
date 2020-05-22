#ifndef POINTOFINTEREST_H
#define POINTOFINTEREST_H

#include "descriptor.h"

#include <qmath.h>


class PointOfInterest
{
public:
    PointOfInterest(int x, int y, double c): x(x), y(y), c(c) {};

    double getC() const {return c;};
    void setC(double value) {c = value;};

    int getX() const {return x;};

    int getY() const {return y;};

    static bool less (PointOfInterest p1, PointOfInterest p2);
    static bool more (PointOfInterest p1, PointOfInterest p2);

    static double distance (PointOfInterest a, PointOfInterest b);

    double getAngle() const;
    void setAngle(double value);

    Descriptor getDescriptor() const;
    void setDescriptor(const Descriptor &value);

private:
    int x, y;
    double c;
    double angle;
    Descriptor descriptor;
};

#endif // POINTOFINTEREST_H
