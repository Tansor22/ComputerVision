#include "mainform.h"
#include <QtMath>
#include "ui_mainform.h"
#include <dataretriver.h>
#include <QFileDialog>
#include <QDebug>
#include <helper.h>


const QString IMAGES_PATH = "C:/Users/Sergei/Documents/QtProjects/images";
const bool saveProportion = true;

double INCREASE_SHARPNESS[] =  {
    -1.0, -1.0, -1.0,
    -1.0, 9.0, -1.0,
    -1.0, -1.0, -1.0
};
double SOBEL_X[] =  {
    1.0, 0, -1.0,
    2.0, 0, -2.0,
    1.0, 0, -1.0
};
double SOBEL_Y[] =  {
    1.0, 2.0, 1.0,
    0.0, 0.0, 0.0,
    -1.0, -2.0, -1.0
};

MainForm::MainForm(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MainForm)
{
    ui->setupUi(this);
    setup();
}
void MainForm::setupGUI() {
    scene = new QGraphicsScene;
    QPixmap pixmap(ui->graphicsView->size());
    QPainter painter(&pixmap);
    // white box
    painter.fillRect(
                QRect(0, 0,
                      ui->graphicsView->width(),
                      ui->graphicsView->height()),
                QBrush(Qt::white));
    // drawing
    scene->addPixmap(pixmap);
    ui->graphicsView->setScene(scene);

    // image drawing logic
    if (fileName == NULL) {
        fileName = QFileDialog::
                getOpenFileName(NULL,
                                "Open Image", IMAGES_PATH, "Image Files (*.png *.jpg *.bmp)");
    }

    //QPixmap imagePixmap;
    imagePixmap.load(fileName);
    if (saveProportion) {
        int w = imagePixmap.width();
        int h = imagePixmap.height();
        double p = h;

        painter.drawPixmap(QRect(0, 0,
                                 ui->graphicsView->width(),
                                 (int) floor(p / w * ui->graphicsView->width())),
                           imagePixmap);
    } else {
        painter.drawPixmap(0, 0, imagePixmap);
    }
    ui->graphicsView->setScene(scene);
}
void MainForm::setup() {
    setupGUI();
    // GAUSSIAN_BLUR
    // should be odd

    double sigma = 3;
    int size = floor(3 * sigma);
    double* kernel =  Helper::gauss(sigma);

    // SOBEL_X

    //int size = 3;
    //double* kernel =  SOBEL_X;

    // SOBEL_Y

    //int size = 3;
    //double* kernel =  SOBEL_Y;


    tool = new ParallelConvolutionalTool(imagePixmap.width(),
                                         imagePixmap.height(),
                                         kernel,
                                         size);

    DataRetriver dr = DataRetriver(NULL);
    int* data = dr.retriveData(imagePixmap);
    int * result = tool->process(data);


    QImage image = imagePixmap.toImage();

    for (int y = 0; y < imagePixmap.height(); y++)
        for (int x = 0; x < imagePixmap.width(); x++)
            image.setPixelColor(x,y,result[y * imagePixmap.width() + x]);

    QFile file("newImage.png");
    file.open(QIODevice::WriteOnly);
    image.save(&file, "PNG");
    scene->addPixmap(QPixmap::fromImage(image));

}
MainForm::~MainForm()
{
    delete ui;
}

