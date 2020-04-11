#ifndef CONSTANTS_H
#define CONSTANTS_H

typedef int Canal;
typedef int FillType;
const FillType ZEROS = 0;
const FillType ONES = 1;
const FillType BORDER = 2;
// Canal must be the grade of 2!
const Canal R = 1;
const Canal G = 2;
const Canal B = 4;
const Canal A = 8;
const Canal GRAY = 16;
#endif // CONSTANTS_H
