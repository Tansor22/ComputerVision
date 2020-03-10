#include <QDebug>

template <class T, unsigned N>
void Print(T (&xArray)[N])
{
    for (unsigned i=0; i<N; i++)
        qDebug() << xArray[i] << ' ';
    qDebug() << endl;
}
void Print(int *xArray) {
    for (unsigned i=0; i< sizeof(xArray) / sizeof (*xArray); i++)
        qDebug() << xArray[i] << ' ';
    qDebug() << endl;
}
