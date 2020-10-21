#include <QDebug>

#include "buzzer.h"
#include <pigpio/pigpio.h>

bool Buzzer::busy = false;

Buzzer::Buzzer()
{
    busy = false;
}

Buzzer::~Buzzer()
{
    qInfo("in Buzzer::~Buzzer");
}

void Buzzer::setPin(int pin)
{
    buzzer_pin = pin;

    if (gpioSetMode(buzzer_pin, PI_OUTPUT) != 0) {
        qInfo("in Buzzer::Buzzer. Error in gpioSetMode pin %d", buzzer_pin);
    }
}

void *Buzzer::_play(void *arg)
{
    int ret;
    int pin = *(int *)arg;

    if ((ret = gpioWrite(pin, 1)) != 0) {
        qInfo("in Buzzer::_play. Error writing pin %d (1)", ret);
    }
    busy = true;
    gpioDelay(500000);
    busy = false;
    if (gpioWrite(pin, 0) !=0) {
        qInfo("in Buzzer::_play. Error writing pin %d (0)", pin);
    }
}

void Buzzer::play() {
    int ret = 0;
    pthread_t *playThread;

    qInfo("in Buzzer::play");
    playThread = gpioStartThread(_play, (void *)&buzzer_pin);
}

bool Buzzer::isPlaying(void)
{
    return busy;
}
