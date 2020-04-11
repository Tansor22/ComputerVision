#include "mainform.h"
#include <QtMath>
#include "ui_mainform.h"
#include <dataretriver.h>
#include <QFileDialog>
#include <QDebug>
#include <helper.h>


MainForm::MainForm(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MainForm)
{
    ui->setupUi(this);
    prepareScene();
}

// fills graphic view with white brush
void MainForm::prepareScene() {
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
    QDesktopWidget* const desktop = QApplication::desktop();
    int w = desktop->width() * .99;
    int h = desktop->height() * .99;
    setWindowTitle("Demo");
    ui->graphicsView->setFixedSize(w, h);

    ui->graphicsView->setScene(scene);


}
MainForm::~MainForm()
{
    delete ui;
}

