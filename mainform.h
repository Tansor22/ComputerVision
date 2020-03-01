#ifndef MAINFORM_H
#define MAINFORM_H

#include <QGraphicsScene>
#include <QWidget>

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
    void setup();
    QString fileName;

private:
    Ui::MainForm *ui;
};
#endif // MAINFORM_H