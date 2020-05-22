#ifndef MAINFORM_H
#define MAINFORM_H

#include "sequentialconvolutiontool.h"
#include "parallelconvolutiontool.h"
#include "convolutiontool.h"
#include <QGraphicsScene>
#include <QWidget>
#include <QDesktopWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class MainForm; }
QT_END_NAMESPACE

class MainForm : public QWidget
{
    Q_OBJECT

public:
    MainForm(QWidget *parent = nullptr);
    ~MainForm();
    QGraphicsScene *scene;
    void prepareScene();

private:
    Ui::MainForm *ui;
};
#endif // MAINFORM_H
