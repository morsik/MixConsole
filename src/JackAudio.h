#ifndef JACKAUDIO_H
#define JACKAUDIO_H

#include <QObject>
#include <QString>
#include <QVector>

#include <jack/jack.h>

class JackAudio;

class JackAudioPort : public QObject
{
    Q_OBJECT

public:
    JackAudioPort(JackAudio *jackClient, const char* portName, unsigned long flags);

    jack_port_t *port;

    float peak;

public slots:
    float getPeak();

signals:
    void peakChanged(float dB);

private:
    const char *name;
    JackAudio *client;
};

class JackAudio : public QObject
{
    Q_OBJECT
public:
    enum PortType
    {
        JACK_INPUT_PORT,
        JACK_OUTPUT_PORT,
    };

    JackAudio(const char* clientName);
    ~JackAudio();

    void createPort(const char* portName, PortType type);
    void deletePort();

    int connectPort(QString sourcePort, QString destinationPort);

    jack_client_t *getClient();

    const char *getName();

    QVector<JackAudioPort *>ports;

private:
    jack_client_t *client;

    const char *name;
};


int jackProcessCallback(jack_nframes_t, void *);

// global JACK object
extern JackAudio *jack;

#endif // JACKAUDIO_H
