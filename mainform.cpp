#include "mainform.h"
#include <QtMath>
#include "ui_mainform.h"
#include <dataretriver.h>
#include <QFileDialog>


const QString IMAGES_PATH = "C:/Users/Sergei/Documents/QtProjects/images";
const bool saveProportion = true;

float INCREASE_SHARPNESS[] =  {
    -1.0f, -1.0f, -1.0f,
    -1.0f, 9.0f, -1.0f,
    -1.0f, -1.0f, -1.0f
};

float GAUSS_BLUR[] = {
    0.000789f, 0.006581f, 0.013347f, 0.006581f, 0.000789f,
    0.006581f, 0.054901f, 0.111345f, 0.054901f, 0.006581f,
    0.013347f, 0.11345f, 0.225821f, 0.111345f, 0.013347f,
    0.006581f, 0.054901f, 0.111345f, 0.054901f, 0.006581f,
    0.000789f, 0.006581f, 0.013347f, 0.006581f, 0.000789f
};

MainForm::MainForm(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MainForm)
{
    ui->setupUi(this);
    setup();
}

void MainForm::setup() {
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
    tool = ConvolutionalTool(GAUSS_BLUR, 5);
    int sample[4] = {
        1, 2,
        3, 4,
    };
    DataRetriver dr = DataRetriver();
    int* data = dr.retriveData(imagePixmap);
    int * result = tool.process(/*2*/ imagePixmap.width(),
                                /*2*/ imagePixmap.height(),
                                /*&sample[0]*/ data);
    ui->graphicsView->setScene(scene);
    QImage image = imagePixmap.toImage();
    for (int y = 0; y < imagePixmap.height(); y++)
        for (int x = 0; x < imagePixmap.width(); x++) {
            image.setPixelColor(x,y,result[y * imagePixmap.width() + x]);
        }
    QFile file("newImage.png");
    file.open(QIODevice::WriteOnly);
    image.save(&file, "PNG");
    scene->addPixmap(QPixmap::fromImage(image));

}
MainForm::~MainForm()
{
    delete ui;
}

