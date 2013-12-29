#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QDebug>

#include <QTimer>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    jack = new JackAudio("MixConsole");

    int _max = 8;
    int i = 0;

    // Create Input Meters

    for (i = 0; i < _max; i++)
    {
        jack->createPort(QString("in_%1").arg(i + 1).toStdString().c_str(), JackAudio::JACK_INPUT_PORT);

        MeterWidget *meter = new MeterWidget();
        meter->setFixedWidth(6);
        meter->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
        this->meters.append(meter);
        ui->inputsGrid->addWidget(meter, 0, i);

        QLabel *label = new QLabel(QString("%1").arg(i + 1));
        ui->inputsGrid->addWidget(label, 1, i);
    }

    QHBoxLayout *inputLabelLayout = new QHBoxLayout();

    QLabel *inputLabel = new QLabel(QString("INPUTS"));
    inputLabel->setAlignment(Qt::AlignHCenter);

    QFrame *inputLabelLine[2];

    for (i = 0; i < 2; i++)
    {
        inputLabelLine[i] = new QFrame();
        inputLabelLine[i]->setFrameShadow(QFrame::Plain);
        inputLabelLine[i]->setStyleSheet(QString("background-color: #f00;"));
        inputLabelLine[i]->setMaximumHeight(1);
    }

    inputLabelLayout->addWidget(inputLabelLine[0]);
    inputLabelLayout->addWidget(inputLabel);
    inputLabelLayout->addWidget(inputLabelLine[1]);
    ui->inputsGrid->addLayout(inputLabelLayout, 2, 0, 1, _max);

    // Create Output (Monitor) Meters
    _max = 8;
    for (i = 0; i < _max; i++)
    {
        jack->createPort(QString("monitor_%1").arg(i + 1).toStdString().c_str(), JackAudio::JACK_INPUT_PORT);

        MeterWidget *meter = new MeterWidget();
        meter->setFixedWidth(6);
        meter->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
        this->meters.append(meter);
        ui->outputsGrid->addWidget(meter, 0, i);

        QLabel *label = new QLabel(QString("%1").arg(i + 1));
        ui->outputsGrid->addWidget(label, 1, i);
    }

    QHBoxLayout *outputLabelLayout = new QHBoxLayout();

    QLabel *outputLabel = new QLabel(QString("OUTPUTS"));
    outputLabel->setAlignment(Qt::AlignHCenter);

    QFrame *outputLabelLine[2];

    for (i = 0; i < 2; i++)
    {
        outputLabelLine[i] = new QFrame();
        outputLabelLine[i]->setFrameShadow(QFrame::Plain);
        outputLabelLine[i]->setStyleSheet(QString("background-color: #f00;"));
        outputLabelLine[i]->setMaximumHeight(1);
    }

    outputLabelLayout->addWidget(outputLabelLine[0]);
    outputLabelLayout->addWidget(outputLabel);
    outputLabelLayout->addWidget(outputLabelLine[1]);
    ui->outputsGrid->addLayout(outputLabelLayout, 2, 0, 1, _max);

//   jack->createPort("Channel Out 1", JackAudio::JACK_OUTPUT_PORT);

    QTimer *tmr = new QTimer();
    tmr->setInterval((1.0f / 16.0f) * 1000);
    tmr->start();

    connect(tmr, SIGNAL(timeout()), this, SLOT(updateMeters()));
}

MainWindow::~MainWindow()
{
    delete ui;
    delete jack;
}

void MainWindow::updateMeters()
{
    int i;
    for (i = 0; i < this->meters.count(); i++)
    {
        this->meters[i]->setValue(jack->ports[i]->getPeak());
    }
}
