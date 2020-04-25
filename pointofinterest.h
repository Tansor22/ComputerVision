#ifndef POINTOFINTEREST_H
#include <qmath.h>
#define POINTOFINTEREST_H


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

private:
    int x, y;
    double c;
};

#endif // POINTOFINTEREST_H
