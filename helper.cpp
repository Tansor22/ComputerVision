#include <QDebug>
#include <QPixmap>
#include <QImage>
#include <constants.h>

//class DataRetriver {
//    public:
//    // order is R G B, offset is w * h
//    int* retriveData(QImage qi, int canals) {
//        int coef = 0;
//        if ((R & canals ) == R) coef++;
//        if ((G & canals ) == G) coef++;
//        if ((B & canals ) == B) coef++;

//        int w = qi.width();
//        int h = qi.height();
//        int* data = new int[w * h * coef];
//        for (int i = 0; i < h; i++)
//            for (int j = 0; j < w; j++) {
//                QRgb rgb = qi.pixel(j,i);
//                if ((R & canals ) == R)
//                    data[i * w + j] = qRed(rgb);
//                 if ((G & canals ) == G)
//                     data[(i * w + j) + w * h] = qGreen(rgb);
//                  if ((B & canals ) == B)
//                      data[(i * w + j) + w * h * 2] = qBlue(rgb);
//            }
//        return data;
//    }

//    int* retriveData(int* arr, int w, int h, int canals) {
//        int coef = 0;
//        if ((R & canals ) == R) coef++;
//        if ((G & canals ) == G) coef++;
//        if ((B & canals ) == B) coef++;

//        int* data = new int[w * h * coef];
//        for (int i = 0; i < h; i++)
//            for (int j = 0; j < w; j++) {
//                QRgb rgb = arr[i * w + j];
//                if ((R & canals ) == R)
//                    data[i * w + j] = qRed(rgb);
//                 if ((G & canals ) == G)
//                     data[(i * w + j) + w * h] = qGreen(rgb);
//                  if ((B & canals ) == B)
//                      data[(i * w + j) + w * h * 2] = qBlue(rgb);
//            }
//        return data;
//    }
//    int* retriveData(QImage qi) {
//        int w = qi.width();
//        int h = qi.height();
//        int* data = new int[w * h];
//        for (int i = 0; i < h; i++)
//            for (int j = 0; j < w; j++)
//                data[i * w + j] = qi.pixel(j,i);
//        return data;
//    }
//    int* retriveData(QPixmap qp) {
//         return this->retriveData(qp.toImage());
//    }

//    int* retriveData(QPixmap qp, int canals) {
//        return this->retriveData(qp.toImage(), canals);
//    }
//};


void printAs2D(int *arr, int rows, int columns) {
    std::string str = "";
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            str += std::to_string(arr[i * columns + j]) + " ";

        }
        str += "\t\n";

    }
    qDebug() << str.c_str() << endl;
}
void printCanals(int *arr, int rows, int columns) {
    std::string str = "";
    for (int c = 0; c < 3; c++){
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
