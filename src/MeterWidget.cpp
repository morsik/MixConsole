#include "MeterWidget.h"
#include "QDebug"

#include <cmath>

#include "utils.h"

MeterWidget::MeterWidget(QWidget *parent) :
    QWidget(parent)
{
    this->peak = 0.0f;
    this->_blockSize = 1;
    this->_spaceSize = 1;

    this->setMinimumHeight(32);
    this->setMinimumWidth(1);
}

void MeterWidget::resizeEvent(QResizeEvent *)
{
    int i = 0;

    int size = this->height();

    // initialize dark graphics
    this->graphicsDark = new QImage(this->width(), size, QImage::Format_ARGB32);
    this->graphicsDark->fill(Qt::transparent);

    // initialize light graphics
    this->graphicsLight = new QImage(this->width(), size, QImage::Format_ARGB32);
    this->graphicsLight->fill(Qt::transparent);

    QPainter darkPainter(this->graphicsDark);
    QPainter lightPainter(this->graphicsLight);

    QBrush lightBrush, darkBrush;

    int _max = this->height() / (this->_blockSize + this->_spaceSize);

    for (i = 0; i < _max; i++)
    {
        float percentPosition = (float)((float)i / (float)_max);
        if (percentPosition < 0.20)
        {
            darkBrush = QColor("#300");
            lightBrush = QColor("#f00");
        }
        else if (percentPosition < 0.35)
        {
            darkBrush = QColor("#330");
            lightBrush = QColor("#ff0");
            darkBrush = QColor("#033");
            lightBrush = QColor("#0ff");
        }
        else
        {
            darkBrush = QColor("#030");
            lightBrush = QColor("#0f0");
            darkBrush = QColor("#033");
            lightBrush = QColor("#0ff");
        }
//        darkBrush = QColor("#300");
//        lightBrush = QColor("#f00");

        QRect blockPosition = QRect(0, i * (this->_blockSize + this->_spaceSize), this->width(), this->_blockSize);

        darkPainter.fillRect(blockPosition, darkBrush);
        lightPainter.fillRect(blockPosition, lightBrush);
    }
}

void MeterWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);

    int max = this->graphicsDark->height() / (this->_blockSize + this->_spaceSize);
    int size = this->_blockSize + this->_spaceSize;
    int height = max * size;
    int split = iec_scale(20.0f * log10f(this->peak), max);

    painter.drawImage(0, 0, this->graphicsDark->copy(0, 0, this->graphicsDark->width(), this->graphicsDark->height() - split * size));
    painter.drawImage(0, height - split * size, this->graphicsLight->copy(0, height - split * size, this->graphicsLight->width(), this->graphicsLight->height()));
}

void MeterWidget::setValue(float dB)
{
    if (dB > this->peak)
    {
        this->peak = dB;
    }
    this->update();
    doFalloff();
}

void MeterWidget::doFalloff()
{
    if (peak < 0.05f)
    {
        this->peak -= 0.009f;
    }
    else if (peak < 0.1f)
    {
        this->peak -= 0.012f;
    }
    else if (peak < 0.2f)
    {
        this->peak -= 0.015f;
    }
    else if (peak < 0.5f)
    {
        this->peak -= 0.02f;
    }
    else
    {
        this->peak -= 0.04f;
    }
}
