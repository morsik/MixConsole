#ifndef METERWIDGET_H
#define METERWIDGET_H

#include <QWidget>
#include <QPainter>
#include <QPaintEvent>
#include <QTimer>

class MeterWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MeterWidget(QWidget *parent = 0);
    ~MeterWidget();

signals:

public slots:
    void setValue(float dB);

private slots:
    void doFalloff();

private:
    void paintEvent(QPaintEvent *);
    void resizeEvent(QResizeEvent *);

    QImage *graphicsLight;
    QImage *graphicsDark;

    float peak;

    int _blockSize;
    int _spaceSize;

    float _last_split;

    bool _first_update;
};

#endif // METERWIDGET_H
