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
    int from;
    int to;
    double *tempCanals;
    double *canals;
    int *target;

public:
    explicit IPWorker(QObject *parent = 0) : QThread(parent) {};
    IPWorker(
            ConvolutionalTool* tool,
            int from,
            int to,
            double *tempCanals,
            double *canals,
            int *target
            ) {
        this->from = from;
        this->to = to;
        this->tempCanals = tempCanals;
        this->canals = canals;
        this->target = target;
        this->tool = tool;
        // looks very unrealiable
        static int id = 0;
        uid = id++;
        name = QString("IPWorker#%1").arg(uid);
    };

    void run() { greet();
                 tool->applyKernel(from, to, tempCanals, canals, target);
                          bye();
               };

    void greet() {qDebug() << "Hello, my name is " << name << endl; }
    void bye() {qDebug() << "Bye, this has been " << name << endl; }

public slots:

signals:

};

#endif // IPWORKER_H
