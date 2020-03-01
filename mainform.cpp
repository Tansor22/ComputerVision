#include "mainform.h"
#include <QtMath>
#include "ui_mainform.h"

#include <QFileDialog>

const QString IMAGES_PATH = "C:/Users/Sergei/Documents/QtProjects/images";
const bool saveProportion = true;

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

    QPixmap imagePixmap;
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
    ui->graphicsView->setScene(scene);


}
MainForm::~MainForm()
{
    delete ui;
}

