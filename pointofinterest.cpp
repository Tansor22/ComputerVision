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
