#include <QDebug>

#include "buzzer.h"
#include <pigpio/pigpio.h>

Buzzer::Buzzer()
{

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

    gpioDelay(500000);

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
