#include "mainform.h"
#include <QtMath>
#include "ui_mainform.h"

#include <QFileDialog>


const QString IMAGES_PATH = "C:/Users/Sergei/Documents/QtProjects/images";
const bool saveProportion = true;

float INCREASE_SHARPNESS_3x3[3][3] =  {
        {1.0f, 1.0f, 1.0f},
        {1.0f, 1.0f, 1.0f},
        {1.0f, 1.0f, 1.0f}
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
    scene->addPixmap(imagePixmap);
    tool = ConvolutionalTool(*INCREASE_SHARPNESS_3x3, 3);
    int sample[4] = {
       1, 2,
       3, 4,
    };
    tool.process(2,2, sample);
    ui->graphicsView->setScene(scene);
    QImage image = imagePixmap.toImage();
    for (int x = 0; x < imagePixmap.width(); x++)
        for (int y = 0; y < imagePixmap.height(); y++) {
            image.setPixelColor(x,y,QRgb());
        }
        QFile file("newImage.png");
        file.open(QIODevice::WriteOnly);
        image.save(&file, "PNG");

}
MainForm::~MainForm()
{
    delete ui;
}

