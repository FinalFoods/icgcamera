#include <QDebug>
#include <pigpio/pigpio.h>

#include "pins.h"
#include "shutter.h"

// Buzzer busy state
bool Shutter::busy;

// Buzzer play time in msec
#define PLAYTIME 500000

Shutter::Shutter()
{
    busy = false;
}

// Set and initialize the buzzer pin
void Shutter::setBuzzerPin(int pin)
{
    buzzer_pin = pin;
    if (gpioSetMode(buzzer_pin, PI_OUTPUT) != 0) {
        qInfo("in Buzzer::Buzzer. Error in gpioSetMode pin %d", buzzer_pin);
    }
}

// Buzzer play private function executed in separate thread
void *Shutter::_buzzerPlay(void *arg)
{
    int ret;
    int pin = *(int *)arg;

    if ((ret = gpioWrite(pin, 1)) != 0) {
        qInfo("in Buzzer::_play. Error writing pin %d (1)", ret);
    }
    busy = true;
    gpioDelay(PLAYTIME);
    busy = false;
    if (gpioWrite(pin, 0) !=0) {
        qInfo("in Buzzer::_play. Error writing pin %d (0)", pin);
    }
}

// Buzzer play public function
void Shutter::buzzerPlay() {
    int ret = 0;
    pthread_t *playThread;

    qInfo("in Buzzer::play");
    playThread = gpioStartThread(_buzzerPlay, (void *)&buzzer_pin);
}

// Returning busy state of buzzer
bool Shutter::isBuzzerPlaying(void)
{
    return busy;
}
