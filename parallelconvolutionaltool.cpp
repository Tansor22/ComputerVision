#include "dataretriver.h"
#include "helper.h"
#include "parallelconvolutionaltool.h"

ParallelConvolutionalTool::ParallelConvolutionalTool(int w, int h, double *kernel, int kernelSize)
    : ConvolutionalTool(w, h, kernel, kernelSize) {}

int* ParallelConvolutionalTool::process(int pixels[]) {
    int *temp = setBounds(BORDER, pixels);
    //printAs2D(temp, tmpH, tmpW);
    double (*mapper)(int);
    mapper = Helper::normalizeStraight;
    Canal type = GRAY;
    DataRetriver dr = DataRetriver(type, mapper);
    tempCanals = dr.retriveData(temp, tmpW, tmpH);
    //printCanals(tempCanals, tmpH, tmpW);

    canals = new double[w * h * 1];
    int* result = new int[w * h];

    const int cores = std::thread::hardware_concurrency();
    int batch = (w * h) / cores;

    // array-pointer to pointers of IPWorker
    IPWorker *workers[cores];
    int workersI = 0;
    for (int i = 0; i <= (w * h) - batch; i += batch) {
        IPWorker *worker = new IPWorker(this, (GRAY & type ) == GRAY, i, i + batch, tempCanals, canals, result);
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
