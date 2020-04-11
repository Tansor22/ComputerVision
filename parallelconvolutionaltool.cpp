#include "dataretriver.h"
#include "helper.h"
#include "parallelconvolutionaltool.h"

int* ParallelConvolutionalTool::process(FillType ft, Canal type, int* pixels) {
    prepare(ft, type, pixels);

    int* result = new int[w * h];

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
