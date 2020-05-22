#include "pointofinterest.h"

bool PointOfInterest::less(PointOfInterest p1, PointOfInterest p2) {
    return  p1.getC() < p2.getC();
}

bool PointOfInterest::more(PointOfInterest p1, PointOfInterest p2) {
    return p1.getC() > p2.getC();
}

double PointOfInterest::distance(PointOfInterest a, PointOfInterest b)
{
    double xd = a.getX() - b.getX();
    double yd = a.getY() - b.getY();
    return  sqrt(xd * xd + yd * yd);
}

double PointOfInterest::getAngle() const
{
    return angle;
}

void PointOfInterest::setAngle(double value)
{
    angle = value;
}

Descriptor PointOfInterest::getDescriptor() const
{
    return descriptor;
}

void PointOfInterest::setDescriptor(const Descriptor &value)
{
    descriptor = value;
}
