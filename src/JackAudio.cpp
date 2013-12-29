#include "JackAudio.h"

#include <cmath>

#include <QDebug>

JackAudio *jack = NULL;

JackAudio::JackAudio(const char *clientName)
{
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
}

JackAudio::~JackAudio()
{
    jack_client_close(this->client);
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

JackAudioPort::JackAudioPort(JackAudio *jackClient, const char *portName, unsigned long flags)
{
    this->name = portName;
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








/*
bool JackAudio::runJACK()
{
//connecting to JACK server
qDebug() << "(open) opening MIDI Client...";
if ((this->jackMidiClient = jack_client_open("MixConsole", JackNullOption, NULL)) == 0)
{
qWarning() << "(open) JACK server not found, aborting...";
exit(false);
}
this->jackMidiPort = jack_port_register(this->jackMidiClient, "QtJack_MIDI_In", JACK_DEFAULT_MIDI_TYPE, JackPortIsInput, 0);

jack_set_process_callback(this->jackMidiClient,
                  jackMidiCallback,
                  NULL);

if (jack_activate(jackMidiClient))
{
qWarning() <<  "(open) jackMidiClient: Can't activate JACK.";
exit(false);
}
qDebug() << "(open) opening Audio Client...";
if ((this->jackAudioClient = jack_client_open("QtJackAudio", JackNullOption, NULL)) == 0)
{
qWarning() << "(open) JACK server not found, aborting...";
exit(false);
}
this->jackAudioPort = jack_port_register(this->jackAudioClient, "QtJack_Audio_Out", JACK_DEFAULT_AUDIO_TYPE, JackPortIsOutput, 0);
jack_set_process_callback(this->jackAudioClient, jackAudioCallback, NULL);
if (jack_activate(this->jackAudioClient))
{
qWarning() <<  "(open) jackAudioClient: Can't activate JACK.";
exit(false);
}
//connecting to audio audio and midi ports
const char** audioPorts;
const char** midiPorts;
if ((audioPorts = jack_get_ports(this->jackAudioClient, NULL, JACK_DEFAULT_AUDIO_TYPE, JackPortIsPhysical|JackPortIsInput)) == NULL)
{
qWarning() << "(open) Cannot find any physical audio playback ports";
//   exit(false);
} else
{
int n = 0;
while (audioPorts[n] != NULL)// connecting to all available physical audio playback ports
{
qDebug() << "(open) Audio output found:" << audioPorts[n];
n++;
}
}
if ((midiPorts = jack_get_ports(this->jackMidiClient, NULL, JACK_DEFAULT_MIDI_TYPE, JackPortIsOutput)) == NULL)
{
qWarning() << "(open) Cannot find any MIDI capture ports";
//   exit(false);
}
else
{
int n = 0;
while (midiPorts[n] != NULL)// connecting to all available physical audio playback ports
{
qDebug() << "(open) MIDI input found:" << midiPorts[n];
n++;
}
}

jack_free(audioPorts);
jack_free(midiPorts);
return(true);
}*/
