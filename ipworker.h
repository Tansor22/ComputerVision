#ifndef IPWORKER_H
#define IPWORKER_H

#include <QObject>
#include <QThread>
#include <QDebug>
#include "convolutiontool.h"

class IPWorker : public QThread
{
    Q_OBJECT
private:
    // meta
    QString name;
    int uid;

    // logic
    ConvolutionTool* tool;
    Canal type;
    int from;
    int to;
    double *tempCanals;
    double *canals;
    QRgb* target;

public:
    explicit IPWorker(QObject *parent = 0) : QThread(parent) {};
    IPWorker(ConvolutionTool* tool, Canal type, int from, int to, double *tempCanals, double *canals, QRgb* target);
    void run();
    void greet() {qDebug() << "Hello, my name is " << name << endl; }
    void bye() {qDebug() << "Bye, this has been " << name << endl; }

public slots:

signals:

};

#endif // IPWORKER_H
