#include "JackAudio.h"

#include <cmath>

#include <QDebug>

JackAudio *jack = NULL;

JackAudio::JackAudio(const char *clientName)
{
    this->name = clientName;

    qDebug() << "(open) opening Audio Client...";

    if ((this->client = jack_client_open(clientName, JackNullOption, NULL)) == 0)
    {
        qWarning() << "(open) JACK server not found, aborting...";
        exit(false);
    }

    jack_set_process_callback(this->client, jackProcessCallback, NULL);

    if (jack_activate(this->client))
    {
        qWarning() <<  "(open) jackAudioClient: Can't activate JACK.";
    }

    qDebug() << "Registerd JACK client with name '" << this->name << "'";
}

JackAudio::~JackAudio()
{
    jack_client_close(this->client);
}

const char *JackAudio::getName()
{
    return this->name;
}

void JackAudio::createPort(const char *portName, JackAudio::PortType portType)
{
    unsigned int flags = 0;

    switch (portType)
    {
        case JackAudio::JACK_INPUT_PORT: flags = JackPortIsInput; break;
        case JackAudio::JACK_OUTPUT_PORT: flags = JackPortIsOutput; break;
    }

    JackAudioPort *port = new JackAudioPort(this, portName, flags);
    this->ports.append(port);
}

jack_client_t *JackAudio::getClient()
{
    return this->client;
}

int JackAudio::connectPort(QString sourcePort, QString destinationPort)
{
    jack_connect(this->client,
                 sourcePort.toStdString().c_str(),
                 destinationPort.toStdString().c_str());
    return 0;
}

JackAudioPort::JackAudioPort(JackAudio *jackClient, const char *portName, unsigned long flags)
{
    this->name = portName;
    this->client = client;
    this->port = jack_port_register(jackClient->getClient(), portName, JACK_DEFAULT_AUDIO_TYPE, flags, 0);
}

float JackAudioPort::getPeak()
{
    float tmp = this->peak;
    this->peak = 0.0f;

    return tmp;
}

int jackProcessCallback(jack_nframes_t nframes, void */*arg*/)
{
    jack_default_audio_sample_t *in;
    unsigned int i;
    int p;

    for (p = 0; p < jack->ports.count(); p++)
    {
        in = (jack_default_audio_sample_t *)jack_port_get_buffer(jack->ports[p]->port, nframes);
        for (i = 0; i < nframes; i++)
        {
            const float s = fabs(in[i]);
            if (s > jack->ports[p]->peak)
            {
                jack->ports[p]->peak = s;
            }
        }
    }

    return 0;
}
