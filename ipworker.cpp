#include "ipworker.h"

IPWorker::IPWorker( ConvolutionalTool* tool,
                    Canal type,
                    int from,
                    int to,
                    double *tempCanals,
                    double *canals,
                    int *target) {
    this->type = type;
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
}

void IPWorker::run() {
    greet();
    tool->applyKernel(Helper::isGray(type), from, to, tempCanals, canals, target);
    bye();
}

