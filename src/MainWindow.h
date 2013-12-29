#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>

#include "JackAudio.h"
#include "MeterWidget.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void updateMeters();

private:
    Ui::MainWindow *ui;

    QVector<MeterWidget *>meters;

};

#endif // MAINWINDOW_H
