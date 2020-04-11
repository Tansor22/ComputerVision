#ifndef IPWORKER_H
#define IPWORKER_H

#include <QObject>
#include <QThread>
#include <QDebug>
#include "convolutionaltool.h"

class IPWorker : public QThread
{
    Q_OBJECT
private:
    // meta
    QString name;
    int uid;

    // logic
    ConvolutionalTool* tool;
    Canal type;
    int from;
    int to;
    double *tempCanals;
    double *canals;
    int *target;

public:
    explicit IPWorker(QObject *parent = 0) : QThread(parent) {};
    IPWorker(ConvolutionalTool* tool, Canal type, int from, int to, double *tempCanals, double *canals, int *target);
    void run();
    void greet() {qDebug() << "Hello, my name is " << name << endl; }
    void bye() {qDebug() << "Bye, this has been " << name << endl; }

public slots:

signals:

};

#endif // IPWORKER_H
