#include "helper.h"

double Helper::normalizeStraight(QRgb rgb) {
    return rgb / 255.0;
}

double Helper::distance(double x1, double x2, double y1, double y2) {
    return sqrt(pow((x2 - x1), 2) + pow((y2 - y1), 2));
}

QList<double> Helper::wrapInQList(double *arr, int size) {
    QList<double> wrapper = QList<double>();
    for (int i = 0; i < size; i++) wrapper.append(arr[i]);
    return wrapper;
}
QString Helper::randomQString() {
    const QString possibleCharacters("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789");
    const int randomStringLength = 12; // assuming you want random strings of 12 characters

    QString randomString;
    for (int i = 0; i < randomStringLength; ++i) {
        int index = qrand() % possibleCharacters.length();
        QChar nextChar = possibleCharacters.at(index);
        randomString.append(nextChar);
    }
    return randomString;
}
QList<QList<double>> Helper::wrapInQListPerCanal(Canal type, double *arr, int w, int h) {
    QList<QList<double>> wrapper = QList<QList<double>>();
    int canalsCount = Helper::canalsCount(type);
    // canals lists
    for (int c = 0; c < canalsCount; c++) {
        QList<double> canal = QList<double>();
        wrapper.append(canal);
    }
    // wrap
    for (int i = 0; i < h; i++)
        for (int j = 0; j < w; j++)
            for (int c = 0; c < canalsCount; c++)
                wrapper[c].append(arr[i * w + j + w * h * c]);
    return wrapper;
}


int Helper::normalizeReverse(double normalized) {
    return static_cast<int>(normalized * 255.0);
}

double *Helper::sample(int from, int to, double *data) {
    // non negative
    assert(from >= 0 && to >= 0);
    int size = to - from;
    // to is greater than from
    assert(size > 0);
    auto *sample = new double[size];
    for (int i = from; i < to; i++)
        sample[i] = data[i];
    return sample;
}

double *Helper::sample(int sampleSize, double *data, int srcSize) {
    // 0 .. (srcSize - sampleSize)
    int from = rand() & (srcSize - sampleSize);
    int to = from + sampleSize;
    return sample(from, to, data);
}

QRgb Helper::supplyWithRGB() {
    int r = (rand() % 256);
    int g = (rand() % 256);
    int b = (rand() % 256);
    return qRgb(r, g, b);
}

double *Helper::getZeroFilledArr(int size) {
    double *doubles = new double[size];
    for (int i = 0; i < size; i++)
        doubles[i] = 0.0;
    return doubles;
}

void Helper::printSample(int from, int to, double *data) {

    qDebug() << gs(from, to, data) << endl;
}

QString Helper::gs(int from, int to, double *data) {
    double *toPrint = sample(from, to, data);
    std::string str = "";
    int elems = 0;

    for (int i = 0; i < to - from; i++) {
        str += std::to_string(toPrint[i]) + " ";
        if (++elems > ELEMENTS_IN_LINE) {
            str += "\t\n";
            elems = 0;
        };
    }
    return str.c_str();
}

void Helper::printSample(int sampleSize, double *data, int srcSize) {
    double *toPrint = sample(sampleSize, data, srcSize);
    std::string str = "";
    int elems = 0;

    for (int i = 0; i < sampleSize; i++) {
        str += std::to_string(toPrint[i]) + " ";
        if (++elems > ELEMENTS_IN_LINE) {
            str += "\t\n";
            elems = 0;
        };
    }
    qDebug() << str.c_str() << endl;
}

double *Helper::gauss(double sigma) {
    int size = floor(3 * sigma);
    double *matrix_gauss = new double[size * size];
    int halfSize = size / 2;
    double ss2 = 2 * sigma * sigma;
    double firstDrob = 1.0 / (M_PI * ss2);
    double test_sum = 0.0;
    int rowI = 0;
    for (int x = -halfSize; x <= halfSize; x++) {
        int columnI = 0;
        for (int y = -halfSize; y <= halfSize; y++) {
            double gauss = firstDrob * exp(-(x * x + y * y) / ss2);
            matrix_gauss[rowI * size + columnI++] = gauss;
            test_sum += gauss;
        }
        rowI++;
    }
    for (int x = -halfSize; x <= halfSize; x++)
        for (int y = -halfSize; y <= halfSize; y++)
            matrix_gauss[(x + halfSize) * size + y + halfSize] /= test_sum;

    //Helper::printAs2D(matrix_gauss, size, size);
    return matrix_gauss;
}

double *Helper::getDoublesTupleForCanals(Canal canals, QRgb rgb, bool normalize) {
    double *tuple = new double[canalsCount(canals)];
    if (isGray(canals)) {
        // single value
        int gray = qGray(rgb);
        tuple[0] = normalize ? normalizeStraight(gray) : gray;
    } else {
        int canalI = 0;
        if ((R & canals) == R) {
            int r = qRed(rgb);
            tuple[canalI++] = normalize ? normalizeStraight(r) : r;;
        }
        if ((G & canals) == G) {
            int g = qGreen(rgb);
            tuple[canalI++] = normalize ? normalizeStraight(g) : g;;
        }
        if ((B & canals) == B) {
            int b = qBlue(rgb);
            tuple[canalI++] = normalize ? normalizeStraight(b) : b;;
        }
        if ((A & canals) == A) {
            int a = qAlpha(rgb);
            tuple[canalI++] = normalize ? normalizeStraight(a) : a;;
        }
    }
    return tuple;
}

QRgb *Helper::toIntRGB(Canal type, double *data, int size) {
    QRgb *output = new QRgb[size];
    for (int i = 0; i < size; i++)
        output[i] =
                isGray(type)
                ? qRgb(normalizeReverse(data[i]),
                       normalizeReverse(data[i]),
                       normalizeReverse(data[i]))
                : qRgba(
                        normalizeReverse(data[i]), // RED
                        normalizeReverse(data[i + size]), // GREEN
                        normalizeReverse(data[i + size * 2]), // BLUE
                        noAlpha(type) ? 255 : normalizeReverse(data[i + size * 3])); // ALPHA
    return output;
}

void Helper::printAs2D(double *arr, int rows, int columns) {
    std::string str = "";
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            str += std::to_string(arr[i * columns + j]) + " ";

        }
        str += "\t\n";

    }
    qDebug() << str.c_str() << endl;
}

double *Helper::copyOf(double *arr, int size) {
    double *output = new double[size];
    for (int i = 0; i < size; i++) output[i] = arr[i];
    return output;
}

void Helper::printCanals(QRgb *arr, int rows, int columns) {
    std::string str = "";
    for (int c = 0; c < 3; c++) {
        str += "Canal " + std::to_string(c + 1) + "\t\n";
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < columns; j++) {
                str += std::to_string(arr[(i * columns + j) + c * rows * columns]) + " ";

            }
            str += "\t\n";
        }
    }
    qDebug() << str.c_str() << endl;
}

bool Helper::isRgb(Canal type) {
    return (R & type) == R && (G & type) == G && (B & type) == B && noAlpha(type);
}
