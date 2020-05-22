#include "dataretriver.h"
#include "helper.h"
#include "parallelconvolutiontool.h"

QRgb*  ParallelConvolutionTool::process(FillType ft, Canal type, QRgb*  pixels) {
    prepare(ft, type, pixels);

    QRgb*  result = new QRgb[w * h];

    const int cores = std::thread::hardware_concurrency();
    int batch = (w * h) / cores;

    // array-pointer to pointers of IPWorker
    IPWorker *workers[cores];
    int workersI = 0;
    for (int i = 0; i <= (w * h) - batch; i += batch) {
        IPWorker *worker = new IPWorker(this, type, i, i + batch, tempCanals, canals, result);
        workers[workersI++] = worker;
    }
    // start
    for (IPWorker *worker : workers)
        worker->start();
    // wait
    for (IPWorker *worker : workers)
        worker->wait();
    // constructing the result
    return result;
}
